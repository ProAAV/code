#include"api_upload.h"
#include<iostream>
#include"fdfs_client.h"
#include"tracker_client.h"
extern "C"{
    #include<libavcodec/avcodec.h>
    #include<libavformat/avformat.h>
    #include<libavutil/avutil.h>
    #include<libswscale/swscale.h>
    #include<libavdevice/avdevice.h>
    #include<libavfilter/avfilter.h>
}
void apiUpload(char* wbuf,int wbuf_sz,struct mg_http_message hm){
    std::cout<<"enter upload api\n";
    CfileInfo file_info{};
    struct mg_str* content_type=mg_http_get_header(&hm,"content-type");
    std::string ct(content_type->buf,content_type->len);
    std::string real_content_type=ct.substr(0,ct.find(';',0));

    std::cout<<"real_content_type:"<<real_content_type<<'\n';
    //将文件信息写入CfileInfo对象的map中储存
    if(real_content_type=="multipart/form-data"){
        struct mg_http_part hpart;
        size_t offset=0;
        while(offset=mg_http_next_multipart(hm.body,offset,&hpart)){
            std::string name(hpart.name.buf,hpart.name.len);
            std::string body(hpart.body.buf,hpart.body.len);
            file_info.getFileInfoMap()[name]=body;
        }
    }
    //先将文件分为多个ts文件和一个m3u8文件
    uploadHandleFileTLS(file_info);
    
}
void uploadHandleFileTLS(CfileInfo& file_info){
    /*avformat_network_init();
    AVFormatContext* input_format_ctx=nullptr;
    std::string file_path=file_info.getFileInfoMap().at("file_path");
    std::cout<<"file_path:"<<file_path<<'\n';
    if(avformat_open_input(&input_format_ctx,file_path.c_str(),nullptr,nullptr)){
        std::cout<<"avformat_open_input failed\n";
        return;
    }
    int ret=avformat_find_stream_info(input_format_ctx,nullptr);
    if(ret<0){
        std::cout<<"avformat_find_stream_info failed\n";
        return;
    }
    AVFormatContext* output_format_ctx=nullptr;
    AVOutputFormat* output_format=av_guess_format("hls",nullptr,nullptr);
    if(!output_format){
        std::cout<<"output_format is null\n";
        return;
    }
    std::string file_md5=file_info.getFileInfoMap().at("file_md5");
    std::string m3u8_file_name=file_md5+".m3u8";
    avformat_alloc_output_context2(&output_format_ctx,output_format,nullptr,m3u8_file_name.c_str());
    if(!output_format_ctx){
        std::cout<<"output_format_ctx is null\n";
        return;
    }
    for(unsigned i=0;i<input_format_ctx->nb_streams;i++){
        AVStream* in_stream=input_format_ctx->streams[i];
        AVStream* out_stream=avformat_new_stream(output_format_ctx,nullptr);
        if(!out_stream){
            std::cout<<"out_stream is null\n";
            return;
        }
        avcodec_parameters_copy(out_stream->codecpar,in_stream->codecpar);
        out_stream->codecpar->codec_tag=0;
    }
    AVDictionary* option=nullptr;

    av_dict_set(&option, "hls_start_number", "0", 0);
    av_dict_set(&option,"hls_time","20",0);
    av_dict_set(&option,"hls_list_size","0",0);
    
    std::string output_file_name=file_md5+"_"+"%03d"+".ts";
    av_dict_set(&option, "hls_segment_filename", output_file_name.c_str(), 0);

    if(!(output_format->flags&AVFMT_NOFILE)){
        if(avio_open(&output_format_ctx->pb,"output.m3u8",AVIO_FLAG_WRITE)<0){
            std::cout<<"avio_open failed\n";
            return;
        }
    }
    AVDictionary* optionn=nullptr;
    //在输出文件写入头时指定字典选项
    ret=avformat_write_header(output_format_ctx,&option);
    if(ret<0){
        std::cout<<"avformat_write_header failed\n";
        return;
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
            return ;
        }

        av_packet_unref(&packet);
    }
    av_write_trailer(output_format_ctx);

    // Close the files
    avio_close(output_format_ctx->pb);
    avformat_close_input(&input_format_ctx);
    avformat_free_context(output_format_ctx);
    std::cout<<"end up\n";*/
    
    //上传文件到fastdfs中
    std::cout<<"enter uploadHandleFileTLS\n";
    uploadFileToFastdfs(file_info);


}

void uploadFileToFastdfs(CfileInfo& file_info){
    std::cout<<"enter uploadFileToFastdfs\n";
    if(fdfs_client_init(FASTDFS_CLIENT_CONF)){
        std::cout<<"err\n";
        return;
    }
    ConnectionInfo* trackerServer=tracker_get_connection();
    if(trackerServer==nullptr){
        fdfs_client_destroy();
        return;
    }
    
    ConnectionInfo* storageServer=new ConnectionInfo();
    memset(storageServer,0,sizeof(ConnectionInfo));
    int storage_path_idx=0;
    
    char* groupName=new char[1024];
    int res=tracker_query_storage_store(trackerServer,storageServer,groupName,&storage_path_idx);
    
    if(!trackerServer||!storageServer){
        std::cout<<"trackerServer is null\n";
        return;
    }
    char* fileId=new char[1024];
    const char* filePath=(char*)"/home/hcc/share/code/project/code/backend/video_/build/fb2c1150ce81974f5a0a6309c58f2a60_000.ts";
    int r=storage_upload_by_filename1(trackerServer,storageServer,\
    storage_path_idx,filePath,nullptr,nullptr,0,groupName,fileId);
    //group1/M00/00/03/wKjQgGe15OyAFfdVABwtxOjf0Ks7803.ts
    std::cout<<"hhhhhhhhhhhhhhh\n";
    std::cout<<"r:"<<r<<'\n';
    
    std::cout<<"fileId:"<<fileId<<"\n";
    tracker_close_connection(trackerServer);
    
    fdfs_client_destroy();
}
