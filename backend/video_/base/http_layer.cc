#include"http_layer.h"
#include<string>
#include<string.h>
#include<iostream>
HttpServer::HttpServer(){

}
HttpServer::~HttpServer()
{

}

void HttpServer::http_parse_(const char* msg){
    struct mg_http_message http_msg;
    mg_http_parse(msg,strlen(msg),&http_msg);
    std::cout<<"-------------------------\n";
    std::cout<<"oooooo:"<<std::string(http_msg.method.buf,http_msg.method.len)<<'\n';
    std::cout<<"-------------------------\n";
}
