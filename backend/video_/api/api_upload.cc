#include"api_upload.h"
#include<iostream>
extern "C"{
    #include<libavcodec/avcodec.h>
    #include<libavformat/avformat.h>
    #include<libavutil/avutil.h>
    #include<libswscale/swscale.h>
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
    uploadHandleFileTLS(file_info);
    
}
void uploadHandleFileTLS(CfileInfo& file_info){
    std::cout<<"2121212121\n";
    avformat_network_init();

}