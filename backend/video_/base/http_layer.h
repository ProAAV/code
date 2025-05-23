#pragma once
#include<string>
#include"config_read.h"
#include"thread_pool.h"
extern "C"{
    #include"mongoose.h"
}
class HttpServer{
public:
    HttpServer(const char* msg,ConfRead& conf_reader);
    ~HttpServer();
    std::string http_parse_method();
    
    
    void http_route_url(char* wbuf,int wbuf_sz,thread_pool_t* thread_pool);
    int http_get_content_length_from_request_header();
    int http_get_header_size();
    std::string http_get_url();
private:
    const char* m_msg;
    struct mg_http_message m_http_msg;
    ConfRead m_conf_reader;
};