#include"http_layer.h"
#include"common.h"
#include"../api/api_upload.h"
#include"../api/api_files_list.h"
#include"../api/api_login.h"
#include"../api/api_register.h"
#include"../api/api_update_user_history.h"
#include"../api/api_user_info.h"
#include"../api/api_search_log.h"
#include"../api/api_search.h"
#include"../api/api_get_search_log.h"
#include"../api/api_list_loop.h"
#include"../api/api_analysis_messages.h"
#include<string.h>
#include<iostream>
HttpServer::HttpServer(const char* msg,ConfRead& conf_reader):m_msg(msg),m_conf_reader(conf_reader){
    if(!m_msg){
        std::cout<<"m_msg is failed to init\n";
    }
    mg_http_parse(m_msg,strlen(m_msg),&m_http_msg);
}
HttpServer::~HttpServer()
{

}
std::string HttpServer::http_get_url(){
    struct mg_str url=m_http_msg.uri;
    std::string real_url(url.buf,url.len);
    std::cout<<"---------------------\n";
    std::cout<<"url:"<<real_url<<"\n";
    std::cout<<"---------------------\n";
    return real_url;
}


void HttpServer::http_route_url(char* wbuf,int wbuf_sz,thread_pool_t* thread_pool){
    //作为路由，url决定调用具体的函数
    std::string url=http_get_url();
    ApiFuncArgs* args=new ApiFuncArgs;
    args->conf_reader=&m_conf_reader;
    args->hm=m_http_msg;
    args->wbuf=wbuf;
    args->wbuf_sz=wbuf_sz;
    if(url=="/api/upload"){
        apiUpload(args);
    }
    else if(url=="/api/filesList"){
        apiFilesList(args);
    }
    else if(url=="/api/login"){
        apiLogin(args);
    }
    else if(url=="/api/register"){
        apiRegister(args);
    }
    else if(url=="/api/uphistory"){
        std::cout<<"url==/api/uphistory\n";
        apiHandleUpdateUserHistory(args);
    }
    else if(url=="/api/userinfo"){
        apiUserInfo(args);
    }
    else if(url=="/api/searchlog"){
        apiSearchLog(args);
    }
    else if(url=="/api/search"){
        apiSearch(args);
    }
    else if(url=="/api/getsearchlog"){
        apiGetSearchLog(args);
    }
    else if(url=="/api/listloop"){
        apiListLoop(args);
    }
    else if(url=="/api/analysismessages"){
        apiAnalysisMessages(args);
    }

}
int HttpServer::http_get_content_length_from_request_header(){
    struct mg_str* content_len=mg_http_get_header(&m_http_msg,"content-length");
    if(!content_len){
        std::cout<<"cant get content-len\n";
        return 1;
    }
    std::string content_length(content_len->buf,content_len->len);
    isNumber(content_length);
    return std::stoi(content_length);
}
int HttpServer::http_get_header_size(){
    std::string msg(m_msg);
    int pos=msg.find("\r\n\r\n",0);
    std::string header=msg.substr(0,pos+4);
    std::cout<<"header size:"<<header.size()<<'\n';
    return header.size();
}
std::string HttpServer::http_parse_method(){
    //如果mg_method没有找到方法的话，后续对他的操作可能会导致程序崩溃，所以要判断，但是程序中还有多处使用mongoose的地方并没有加以判断，后续要补上
    
    struct mg_str mg_method=m_http_msg.method;
    if(mg_method.buf==nullptr||mg_method.len==0){
        std::cout<<"err\n";
        return "";
    }
    std::string method(mg_method.buf,mg_method.len);
    if(method=="GET"){
        std::cout<<"GET method\n";
        return "GET";
    }
    else if(method=="POST"){
        std::cout<<"POST method\n";
        return "POST";
    }
    else{
        std::cout<<"unkown method was recv\n";
        return "UNKOWN";
    }
}
