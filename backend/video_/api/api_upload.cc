#include"api_upload.h"
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<thread>
#include<json/json.h>
#include"../base/common.h"
#include"fdfs_client.h"
#include"tracker_client.h"
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/py_script.h"
extern "C"{
    #include<libavcodec/avcodec.h>
    #include<libavformat/avformat.h>
    #include<libavutil/avutil.h>
    #include<libswscale/swscale.h>
    #include<libavdevice/avdevice.h>
    #include<libavfilter/avfilter.h>
    #include<libavutil/imgutils.h>
    #include<mysql/mysql.h>
}
std::string video_analysis_message;

void upload_responSuccess(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]="0";
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, root);
    std::string response="HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(jsonStr.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            jsonStr;
    if(jsonStr.size()>wbuf_sz){
        std::cout<<"size is too small\n";
        return;
    }
    memcpy(wbuf,response.c_str(),response.size());
}
void upload_responFailed(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]="1";
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, root);
    std::string response="HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(jsonStr.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            jsonStr;
    if(jsonStr.size()>wbuf_sz){
        std::cout<<"size is too small\n";
        return;
    }
    memcpy(wbuf,response.c_str(),response.size());
}
void apiUpload(void* args){
    std::cout<<"enter upload api\n";
    //文件信息是由我们自己根据http请求体发送来的表单字段解析得到的
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;
    
    CfileInfo file_info{};
    struct mg_str* content_type=mg_http_get_header(&hm,"content-type");
    std::string ct(content_type->buf,content_type->len);
    std::string real_content_type=ct.substr(0,ct.find(';',0));

    std::cout<<"real_content_type:"<<real_content_type<<'\n';
    //解析http请求体的表单字段，将文件信息写入CfileInfo对象的map中储存
    if(real_content_type=="multipart/form-data"){
        struct mg_http_part hpart;
        size_t offset=0;
        while(offset=mg_http_next_multipart(hm.body,offset,&hpart)){
            std::string name(hpart.name.buf,hpart.name.len);
            std::string body(hpart.body.buf,hpart.body.len);
            file_info.getFileInfoMap()[name]=body;
        }
    }
    else{
        std::cout<<"文件上传失败，文件内容不匹配\n";
        upload_responFailed(wbuf,wbuf_sz);
        return;
    }
    //先来判断文件的md5信息，如果数据库中已经存在相同的md5，那么直接更新数据库表即可，如果没有则进行常规的文件上传操作
    if(fileMd5InfoCheckout(file_info)){
        
        upload_responSuccess(wbuf,wbuf_sz);
        return;
    }
   

    //先将文件分为多个ts文件和一个m3u8文件
    if(uploadHandleFileTLS(file_info,conf_reader)){
        upload_responFailed(wbuf,wbuf_sz);
        return;
    }
    upload_responSuccess(wbuf,wbuf_sz);
    return;
}
bool fileMd5InfoCheckout(CfileInfo& file_info){
    std::cout<<"enter fileMd5InfoCheckout\n";
    std::string file_md5=file_info.getFileInfoMap().at("file_md5");
    std::string file_title=file_info.getFileInfoMap().at("file_title");
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select * from aav_file_info where file_md5 = '%s'",file_md5.c_str());
    std::cout<<"sql:"<<query_<<'\n';
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(mysql_num_rows(res)){
        //如果找到了一条记录，先看看当前用户是否已经有一条该文件记录;
        std::string username=file_info.getFileInfoMap().at("username");
        sprintf(query_,"select * from aav_user_file where username='%s' and file_md5='%s'",username.c_str(),file_md5.c_str());
        res=sql_conn.mysqlQuery(query_);
        if(mysql_num_rows(res)){
            std::cout<<"user table have exist\n";
            mysql_free_result(res);
            return true;
        }
        //如果该用户没有该文件的一条记录，那么更新数据库表再return就行了；
        sprintf(query_,"insert into aav_user_file (username,file_md5,date_time,file_title) value('%s','%s',now(),'%s')",username.c_str(),file_md5.c_str(),file_title.c_str());
        std::cout<<"sql:"<<query_<<'\n';
        sql_conn.mysqlQuery(query_);
        mysql_free_result(res);
        return true;
    }
    //没找到就走正常流程
    mysql_free_result(res);
    return false;
}
//检查字符串是否以'/'结尾，如果不是则添加一个'/'
void checkEnd(std::string& str){
    char c=str.back();
    if(c!='/'){
        str.push_back('/');
    }
    else{
        return;
    }
    return;
}
int uploadHandleFileTLS(CfileInfo& file_info,ConfRead& conf_reader){
    avformat_network_init();
    AVFormatContext* input_format_ctx=nullptr;
    std::string file_path=file_info.getFileInfoMap().at("file_path");
    std::cout<<"file_path:"<<file_path<<'\n';
    if(avformat_open_input(&input_format_ctx,file_path.c_str(),nullptr,nullptr)){
        std::cout<<"avformat_open_input failed\n";
        return 1;
    }
    int ret=avformat_find_stream_info(input_format_ctx,nullptr);
    if(ret<0){
        std::cout<<"avformat_find_stream_info failed\n";
        return 1;
    }
    AVFormatContext* output_format_ctx=nullptr;
    AVOutputFormat* output_format=av_guess_format("hls",nullptr,nullptr);
    if(!output_format){
        std::cout<<"output_format is null\n";
        return 1;
    }
    //指定m3u8文件的命名规则,命名规则就直接指定了ts文件的存储路径
    std::string ts_and_m3u8_and_pic_dir=conf_reader.confGetMap().at("ts_and_m3u8_and_pic_dir");
    //------------------------------------昨天想到这里，思路是为每一个ts文件和m3u8文件通过读取配置文件中的目录，放到这个目录下\
    然后使用fork创建一个子进程去做exec删除一个目录下的所有文件，做到文件的批量删除
    //先创建目录
    char* command=new char[124];
    sprintf(command,"mkdir -p %s",ts_and_m3u8_and_pic_dir.c_str());
    if(system(command)==-1){
        std::cout<<command<<" is failed\n";
        return 1;
    }
    //再拼凑整个完整路径。
    checkEnd(ts_and_m3u8_and_pic_dir);
    std::string file_md5=file_info.getFileInfoMap().at("file_md5");
    std::string m3u8_file_name=ts_and_m3u8_and_pic_dir+file_md5+".m3u8";
    std::cout<<"-----m3u8_file_name:"<<m3u8_file_name<<"\n";
    avformat_alloc_output_context2(&output_format_ctx,output_format,nullptr,m3u8_file_name.c_str());
    if(!output_format_ctx){
        std::cout<<"output_format_ctx is null\n";
        return 1;
    }
    for(unsigned i=0;i<input_format_ctx->nb_streams;i++){
        AVStream* in_stream=input_format_ctx->streams[i];
        AVStream* out_stream=avformat_new_stream(output_format_ctx,nullptr);
        if(!out_stream){
            std::cout<<"out_stream is null\n";
            return 1;
        }
        avcodec_parameters_copy(out_stream->codecpar,in_stream->codecpar);
        out_stream->codecpar->codec_tag=0;
    }
    AVDictionary* option=nullptr;

    av_dict_set(&option, "hls_start_number", "0", 0);
    av_dict_set(&option,"hls_time","20",0);
    av_dict_set(&option,"hls_list_size","0",0);
    //指定ts文件的命名规则

    std::string output_file_name=ts_and_m3u8_and_pic_dir+file_md5+"_"+"%03d"+".ts";
    std::cout<<"-----output_file_name_ts:"<<output_file_name<<'\n';
    av_dict_set(&option, "hls_segment_filename", output_file_name.c_str(), 0);

    if(!(output_format->flags&AVFMT_NOFILE)){
        if(avio_open(&output_format_ctx->pb,"output.m3u8",AVIO_FLAG_WRITE)<0){
            std::cout<<"avio_open failed\n";
            return 1;
        }
    }
    AVDictionary* optionn=nullptr;
    //在输出文件写入头时指定字典选项
    ret=avformat_write_header(output_format_ctx,&option);
    if(ret<0){
        std::cout<<"avformat_write_header failed\n";
        return 1;
    }
    AVPacket packet;
    av_init_packet(&packet);
    packet.data=nullptr;
    packet.size=0;
    while(av_read_frame(input_format_ctx,&packet)>=0){
        AVStream* i_stream=input_format_ctx->streams[packet.stream_index];
        AVStream* o_stream = output_format_ctx->streams[packet.stream_index];
        packet.pts = av_rescale_q_rnd(packet.pts, i_stream->time_base, o_stream->time_base, AV_ROUND_NEAR_INF);
        packet.dts = av_rescale_q_rnd(packet.dts, i_stream->time_base, o_stream->time_base, AV_ROUND_NEAR_INF);
        packet.duration = av_rescale_q(packet.duration, i_stream->time_base, o_stream->time_base);
        packet.pos = -1;

        // Write the packet
        if (av_interleaved_write_frame(output_format_ctx, &packet) != 0) {
            std::cerr << "Error while writing output packet." << std::endl;
            return 1;
        }

        av_packet_unref(&packet);
    }
    av_write_trailer(output_format_ctx);

    // Close the files
    avio_close(output_format_ctx->pb);
    avformat_close_input(&input_format_ctx);
    avformat_free_context(output_format_ctx);
    std::cout<<"end up\n";
    //生成一张封面图片
    bool is_audio;
    generateCoverPic(file_info,conf_reader,is_audio);
    //先判断文件中有无视频流，如果没有就直接判定为音频流，那么就可以调用py接口分析标签,可以考虑异步执行
    std::cout<<"out generateCoverPic\n";
    if(is_audio){

        /*PyScript pyscript{};
        pyscript.pyscrpt_init();
        std::string strr=pyscript.pyscrpt_audio_interface(file_path);
        upload_extract_file_features(strr);
        std::cout<<"pyscrpt_audio_interface----------\n";*/
        std::thread thrd(audio_py_update,file_info);
        thrd.detach();
        //audio_py_update(file_path);
    }
    else if(!is_audio){
        /*PyScript pyscript{};
        pyscript.pyscrpt_init();
        video_analysis_message=pyscript.pyscrpt_video_interface(file_path);

        //upload_video_extract_analysis(strr);
        std::cout<<"pyscrpt_video_interface----------\n";*/
        std::thread thrd(video_py_update,file_info);
        thrd.detach();
        //video_py_update(file_path);
    }
    //上传文件到fastdfs中
    std::cout<<"enter uploadHandleFileTLS\n";
    //在这之前就是生成ts文件和m3u8文件和视封面图片文件，存放到指定的/tmp文件夹下，之后应该是修改m3u8文件中的ts文件路径
    if(uploadFileToFastdfs(file_info,conf_reader,is_audio)){
        return 1;
    }
    return 0;

}
void audio_py_update(CfileInfo file_info){
    std::string file_path=file_info.getFileInfoMap().at("file_path");
    std::string file_md5=file_info.getFileInfoMap().at("file_md5");
    PyScript pyscript{};
    pyscript.pyscrpt_init();
    std::string strr=pyscript.pyscrpt_audio_interface(file_path);
    upload_extract_file_features(strr);
    char sql[512]={0};
    sprintf(sql,"update aav_file_info set file_feature_1='%s',file_feature_2='%s',file_feature_3='%s',file_feature_4='%s',file_feature_5='%s' where file_md5='%s'",vec_file_features[0].c_str(),vec_file_features[1].c_str(),vec_file_features[2].c_str(),vec_file_features[3].c_str(),vec_file_features[4].c_str(),file_md5.c_str());
    std::cout<<"sqll:"<<sql<<'\n';
    MysqlConn conn{};
    conn.mysqlQuery(sql);
    std::cout<<"pyscrpt_audio_interface----------\n";
}
void video_py_update(CfileInfo file_info){
    std::string file_path=file_info.getFileInfoMap().at("file_path");
    std::string file_md5=file_info.getFileInfoMap().at("file_md5");
    PyScript pyscript{};
    pyscript.pyscrpt_init();
    video_analysis_message=pyscript.pyscrpt_video_interface(file_path);
    char sql[8192]={0};
    sprintf(sql,"update aav_video_descrpt set message='%s'where file_md5='%s'",video_analysis_message.c_str(),file_md5.c_str());
    MysqlConn conn{};
    conn.mysqlQuery(sql);
    //upload_video_extract_analysis(strr);
    std::cout<<"pyscrpt_video_interface----------\n";
}
void upload_extract_file_features(std::string str){
    std::cout<<"enter upload_extract_file_features\n";
    if(str==""){
        std::cout<<"upload_extract_file_features invalid params\n";
        return;
    }
    std::cout<<"str raw:"<<str<<'\n';
    
    std::cout<<"str:"<<str<<"\n";
    int s_pos=0;
    int pos=0;
    vec_file_features.clear();
    
    do{
        pos=str.find(",",s_pos);
        std::string feature=str.substr(s_pos,pos-s_pos);
        std::cout<<"s_pos:"<<s_pos<<" "<<"pos-s_pos:"<<pos-s_pos<<"\n";
        std::cout<<"str_tags:"<<feature<<'\n';
        vec_file_features.push_back(feature);
        s_pos=pos+1;
    }while(pos!=std::string::npos);
    
    std::cout<<"end upload_extract_file_features\n";
    return;
    
}
std::string upload_video_extract_analysis(std::string str){
    std::cout<<"upload_video_extract_analysis str::"<<str<<'\n';
    int curr_head_pos=0;
    int curr_tail_pos=0;
    int start_pos=str.find("\n\n",0);
    int pos=0;
    while((curr_head_pos=str.find("\n\n",curr_head_pos+2))!=std::string::npos){
        pos=curr_head_pos;
        std::cout<<"curr_head_pos:"<<curr_head_pos<<'\n';
    };
    if(start_pos>str.size()||pos-start_pos>str.size()-start_pos){
        std::cout<<"lllllllll\n";
        std::cout<<start_pos<<'\n';
        std::cout<<str.size()<<'\n';
        std::cout<<pos-start_pos<<'\n';
        std::cout<<str.size()-start_pos<<'\n';

        return "";
    }
    return str.substr(start_pos,pos-start_pos);
    //2 , 3
    //0,1,2,3,4
    /*for(int i=0;i<4;i++){
        curr_head_pos=str.find("**",curr_tail_pos);
        if(str.find("\r\n\r\n",curr_head_pos)==std::string::npos){
            curr_tail_pos=str.find("\n\n",curr_head_pos);
        }
        else{
            curr_tail_pos=str.find("\r\n\r\n",curr_head_pos);
        }
        //str.substr(curr_pos,curr_tail_pos-curr_head_pos);
        std::string ss=str.substr(curr_head_pos,curr_tail_pos-curr_head_pos);
        std::cout<<"ss:"<<ss<<std::endl;
        vec_file_analysis.push_back(ss);
        curr_head_pos=curr_tail_pos+1;
    }
    for(auto s:vec_file_analysis){
        std::cout<<"anal:"<<s<<'\n';
    }*/
    

}

int generateCoverPic(CfileInfo& file_info,ConfRead& conf_reader,bool& is_audio) {
    AVFormatContext* format_context = nullptr;
    AVCodecParameters* decoder_param = nullptr;
    AVCodecContext* decoder_context = nullptr;
    AVCodecContext* enc_ctx = nullptr;
    AVFrame* frame = nullptr;
    AVFrame* rgb_frame = nullptr;
    SwsContext* swsContext = nullptr;
    AVCodec* decodec = nullptr;
    const AVCodec* png_codec = nullptr;
    std::string file_path=file_info.getFileInfoMap().at("file_path");
    int ret = avformat_open_input(&format_context, file_path.c_str(), nullptr, nullptr);
    if (ret != 0) {
        fprintf(stderr, "Failed to open input file\n");
        return 1;
    }

    ret = avformat_find_stream_info(format_context, nullptr);
    if (ret < 0) {
        avformat_close_input(&format_context);
        fprintf(stderr, "Failed to find stream info\n");
        return 1;
    }

    int video_stream_index = -1;
    for (unsigned i = 0; i < format_context->nb_streams; i++) {
        if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    is_audio=false;
    if (video_stream_index == -1) {
        avformat_close_input(&format_context);
        fprintf(stderr, "No video stream found\n");
        is_audio=true;
        return 1;
    }

    decoder_param = format_context->streams[video_stream_index]->codecpar;
    decodec = avcodec_find_decoder(decoder_param->codec_id);
    if (!decodec) {
        avformat_close_input(&format_context);
        fprintf(stderr, "Decoder not found\n");
        return 1;
    }

    decoder_context = avcodec_alloc_context3(decodec);
    if (!decoder_context) {
        avformat_close_input(&format_context);
        fprintf(stderr, "Failed to allocate decoder context\n");
        return 1;
    }

    ret = avcodec_parameters_to_context(decoder_context, decoder_param);
    if (ret < 0) {
        avcodec_free_context(&decoder_context);
        avformat_close_input(&format_context);
        fprintf(stderr, "Failed to copy codec parameters to decoder context\n");
        return 1;
    }

    ret = avcodec_open2(decoder_context, decodec, nullptr);
    if (ret < 0) {
        avcodec_free_context(&decoder_context);
        avformat_close_input(&format_context);
        fprintf(stderr, "Failed to open decoder\n");
        return 1;
    }

    frame = av_frame_alloc();
    rgb_frame = av_frame_alloc();
    if (!frame || !rgb_frame) {
        av_frame_free(&frame);
        av_frame_free(&rgb_frame);
        avcodec_free_context(&decoder_context);
        avformat_close_input(&format_context);
        fprintf(stderr, "Failed to allocate frame(s)\n");
        return 1;
    }

    AVPacket packet;
    int frame_count = 0;

    while (av_read_frame(format_context, &packet) >= 0) {
        if (packet.stream_index != video_stream_index) {
            av_packet_unref(&packet);
            continue;
        }

        ret = avcodec_send_packet(decoder_context, &packet);
        if (ret < 0) {
            av_packet_unref(&packet);
            break;
        }

        while (avcodec_receive_frame(decoder_context, frame) == 0) {
            frame_count++;
            if (frame_count > 1) break;

            swsContext = sws_getContext(
                decoder_context->width,
                decoder_context->height,
                decoder_context->pix_fmt,
                decoder_context->width,
                decoder_context->height,
                AV_PIX_FMT_RGB24,
                SWS_BILINEAR, nullptr, nullptr, nullptr
            );
            if (!swsContext) {
                fprintf(stderr, "Failed to create sws context\n");
                break;
            }

            int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, decoder_context->width, decoder_context->height, 1);
            uint8_t* buffer = (uint8_t*)av_malloc(numBytes);
            if (!buffer) {
                fprintf(stderr, "Failed to allocate buffer\n");
                break;
            }

            av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, buffer, AV_PIX_FMT_RGB24, decoder_context->width, decoder_context->height, 1);

            // Explicitly set the necessary frame properties
            rgb_frame->format = AV_PIX_FMT_RGB24;
            rgb_frame->width = decoder_context->width;
            rgb_frame->height = decoder_context->height;

            int scaled_lines = sws_scale(swsContext, frame->data, frame->linesize, 0, decoder_context->height, rgb_frame->data, rgb_frame->linesize);
            sws_freeContext(swsContext);
            if (scaled_lines <= 0) {
                av_free(buffer);
                fprintf(stderr, "Color conversion failed\n");
                break;
            }

            png_codec = avcodec_find_encoder(AV_CODEC_ID_PNG);
            if (!png_codec) {
                av_free(buffer);
                fprintf(stderr, "PNG encoder not found\n");
                break;
            }

            enc_ctx = avcodec_alloc_context3(png_codec);
            if (!enc_ctx) {
                av_free(buffer);
                fprintf(stderr, "Failed to allocate encoder context\n");
                break;
            }

            enc_ctx->width = decoder_context->width;
            enc_ctx->height = decoder_context->height;
            enc_ctx->pix_fmt = AV_PIX_FMT_RGB24;
            enc_ctx->time_base = {1, 1};  // Ensure appropriate time base

            ret = avcodec_open2(enc_ctx, png_codec, nullptr);
            if (ret < 0) {
                avcodec_free_context(&enc_ctx);
                av_free(buffer);
                fprintf(stderr, "Failed to open encoder\n");
                break;
            }

            AVPacket pkt;
            av_init_packet(&pkt);
            pkt.data = nullptr;
            pkt.size = 0;

            ret = avcodec_send_frame(enc_ctx, rgb_frame);
            if (ret < 0) {
                char errbuf[256];
                av_strerror(ret, errbuf, sizeof(errbuf));
                fprintf(stderr, "Send frame error: %s\n", errbuf);
            } else {
                ret = avcodec_receive_packet(enc_ctx, &pkt);
                if (ret == 0) {
                    std::string dir=conf_reader.confGetMap().at("ts_and_m3u8_and_pic_dir");
                    checkEnd(dir);
                    std::string file_name=file_info.getFileInfoMap().at("file_md5");
                    std::string out_file_path=dir+file_name+".png";
                    std::cout<<"------out_file_path_pic:"<<out_file_path<<'\n';
                    FILE* fp = fopen(out_file_path.c_str(), "wb");
                    if (fp) {
                        fwrite(pkt.data, 1, pkt.size, fp);
                        fclose(fp);
                        printf("Successfully saved frame\n");
                    } else {
                        fprintf(stderr, "Failed to open output file\n");
                    }
                } else if (ret != AVERROR(EAGAIN)) {
                    fprintf(stderr, "Receive packet error\n");
                }
            }

            av_packet_unref(&pkt);
            avcodec_free_context(&enc_ctx);
            av_free(buffer);
            break;
        }

        av_packet_unref(&packet);
        if (frame_count > 1) break;
    }

    av_frame_free(&frame);
    av_frame_free(&rgb_frame);
    avcodec_free_context(&decoder_context);
    avformat_close_input(&format_context);
    return 0;
}
int uploadFileToFastdfs(CfileInfo& file_info,ConfRead& conf_reader,const bool& is_audio){
    //这个函数主要就是先把ts文件上传到fastdfs后修改m3u8文件中的ts文件地址，最后把自己这个m3u8文件上传到fastdfs中就行了
    std::cout<<"enter uploadFileToFastdfs\n";
    if(fdfs_client_init(FASTDFS_CLIENT_CONF)){
        std::cout<<"err\n";
        return 1;
    }
    ConnectionInfo* trackerServer=tracker_get_connection();
    if(trackerServer==nullptr){
        fdfs_client_destroy();
        return 1;
    }
    
    ConnectionInfo* storageServer=new ConnectionInfo();
    memset(storageServer,0,sizeof(ConnectionInfo));
    int storage_path_idx=0;
    
    char* groupName=new char[1024];
    int res=tracker_query_storage_store(trackerServer,storageServer,groupName,&storage_path_idx);
    
    if(!trackerServer||!storageServer){
        std::cout<<"trackerServer is null\n";
        return 1;
    }
    //得到ts这些文件的文件夹路径
    std::string ts_and_m3u8_and_pic_dir=conf_reader.confGetMap().at("ts_and_m3u8_and_pic_dir");
    checkEnd(ts_and_m3u8_and_pic_dir);
    //得到m3u8的文件路径
    std::string m3u8_file_name=ts_and_m3u8_and_pic_dir+file_info.getFileInfoMap().at("file_md5")+".m3u8";
    //读取m3u8文件获取ts文件列表的文件名
    std::ifstream m3u8_file(m3u8_file_name);
    std::cout<<"file_name:"<<m3u8_file_name<<"\n";
    if(!m3u8_file.is_open()){
        std::cout<<"m3u8_file is not open\n";
        return 1;
    }
    std::string line="";
    std::string ts_file_name="";
    std::vector<std::string> file_content;
    while(std::getline(m3u8_file,line)){
        if(line.at(0)=='#'){
            file_content.push_back(line);
            continue;
        }
        ts_file_name=ts_and_m3u8_and_pic_dir+line;
        char fileId[1024];
        int r=storage_upload_by_filename1(trackerServer,storageServer,\
        storage_path_idx,ts_file_name.c_str(),nullptr,nullptr,0,groupName,fileId);
        std::string addr_nginx=conf_reader.confGetMap().at("addr_of_nginx");
        if(addr_nginx.back()!='/')addr_nginx.push_back('/');
        line=addr_nginx+std::string(fileId);
        
        file_content.push_back(line);
    }
    m3u8_file.close();
    std::ofstream m3u8_file_write(m3u8_file_name);
    if(!m3u8_file_write.is_open()){
        std::cout<<"m3u8_file_write open failed\n";
        return 1;
    }
    //写回文件
    for(auto& i:file_content){
        
        m3u8_file_write<<i<<'\n';
    }
    m3u8_file_write.close();

    //现在该上传m3u8文件了
    char file_id[1024];
    int r=storage_upload_by_filename1(trackerServer,storageServer,\
    storage_path_idx,m3u8_file_name.c_str(),nullptr,nullptr,0,groupName,file_id);
    //最重要的是这个m3u8文件的id，因为之后客户端请求的只有这个m3u8文件，所以要将这个文件更新到mysql数据库中存储
    std::cout<<"===================id:"<<file_id<<'\n';
    std::string addr_of_nginx=conf_reader.confGetMap().at("addr_of_nginx");
    checkEnd(addr_of_nginx);
    std::string update_file_path=addr_of_nginx+std::string(file_id);
    std::string update_file_md5=file_info.getFileInfoMap().at("file_md5");
    std::string update_file_size=file_info.getFileInfoMap().at("file_size");


    std::string update_file_title=file_info.getFileInfoMap().at("file_title");
    std::string update_file_playback_duration=file_info.getFileInfoMap().at("file_playback_duration");

    
    
    //再然后上传png图片文件
    std::string update_file_image_path="null";
    if(!is_audio){
        std::string pic_file_name=ts_and_m3u8_and_pic_dir+file_info.getFileInfoMap().at("file_md5")+".png";
        r=storage_upload_by_filename1(trackerServer,storageServer,\
        storage_path_idx,pic_file_name.c_str(),nullptr,nullptr,0,groupName,file_id);
        //其次是这个png图片的id也是很重要的，因为没有其他办法找到这个图片的文件位置，只能将返回的id用mysql存储起来
        std::cout<<"===================pngid:"<<file_id<<'\n';
        update_file_image_path=addr_of_nginx+std::string(file_id);
    }
    
    
    //得到数据之后更新aav_file_info这张表
    char query[512];
    MysqlConn conn{};
    /*if(vec_file_features.size()!=5&&is_audio){
        std::cout<<"vec_file_features size:"<<vec_file_features.size()<<'\n';
        for(auto i:vec_file_features){
            std::cout<<"feature:"<<i<<'\n';
        }
        std::cout<<"vec_file_features.size()!=5\n";
        return 1;
    }*/
    std::string file_type;
    if(is_audio){
        //"0"表示音频文件
        file_type="0";
        /*std::string emotion=vec_file_features.at(4);
        emotion.find("：",0);
        std::string str_emotion=emotion.substr(emotion.find("：",0)+3,std::string::npos);
        std::cout<<"emotion str:"<<str_emotion<<'\n';
        vec_file_features[4]=str_emotion;*/
    }
    else{
        file_type="1";
        
    }
    //vec_file_features.clear();
    //for(int i=0;i<5;i++)vec_file_features.push_back("");
    //sprintf(query,"insert into `aav_file_info` (file_path,file_md5,file_size,file_playback_duration,file_image_path,file_feature_1,file_feature_2,file_feature_3,file_feature_4,file_feature_5,file_type) value('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",update_file_path.c_str(),update_file_md5.c_str(),update_file_size.c_str(),update_file_playback_duration.c_str(),update_file_image_path.c_str(),vec_file_features[0].c_str(),vec_file_features[1].c_str(),vec_file_features[2].c_str(),vec_file_features[3].c_str(),vec_file_features[4].c_str(),file_type.c_str());
    sprintf(query,"insert into `aav_file_info` (file_path,file_md5,file_size,file_playback_duration,file_image_path,file_feature_1,file_feature_2,file_feature_3,file_feature_4,file_feature_5,file_type) value('%s','%s','%s','%s','%s','','','','','','%s')",update_file_path.c_str(),update_file_md5.c_str(),update_file_size.c_str(),update_file_playback_duration.c_str(),update_file_image_path.c_str(),file_type.c_str());

    std::cout<<"query::::"<<query<<"\n";
    
    conn.mysqlQuery(query);
    //更新aav_video_descrpt这张表，将视频文件的描述插入
    std::string username=file_info.getFileInfoMap().at("username");
    //std::string sstr=upload_video_extract_analysis(video_analysis_message);
    if(!is_audio){
        std::string sstr="";
        std::cout<<"sstr-----------======:"<<sstr<<std::endl;
        sprintf(query,"insert into aav_video_descrpt (username,file_md5,message) value('%s','%s','%s')",username.c_str(),update_file_md5.c_str(),sstr.c_str());
        std::cout<<"analysis query:"<<query<<std::endl;
        conn.mysqlQuery(query);
    }

    //之后更新aav_user_file这张表
    
    sprintf(query,"insert into `aav_user_file` (username,file_md5,date_time,file_title) value('%s','%s',now(),'%s')",username.c_str(),update_file_md5.c_str(),update_file_title.c_str());
    conn.mysqlQuery(query);

    tracker_close_connection(trackerServer);
    
    fdfs_client_destroy();
    //上传完毕以后将tmp目录下的内容清空，这个内容就是一次文件上传所产生的文件
    if(removeFilesInTmp(conf_reader)){
        return 1;
    }
    std::cout<<"out dfs upload\n";
    return 0;
    
}
int removeFilesInTmp(ConfRead& conf_reader){
    std::string command="rm -rf "+conf_reader.confGetMap().at("ts_and_m3u8_and_pic_dir");
    if(system(command.c_str())){
        return 1;
    }
    command="rm -f "+std::string("/tmp/nginx_upload/*");
    if(system(command.c_str())){
        return 1;
    }
    return 0;
}
