#pragma once
#include<string>
extern "C"{
    #include"mongoose.h"
}
class HttpServer{
public:
    HttpServer(const char* msg);
    ~HttpServer();
    std::string http_parse_method();
    void http_parse_get();
    void http_parse_post(char* wbuf,int wbuf_sz);
    int http_get_content_length_from_request_header();
    int http_get_header_size();
    std::string http_get_url();
private:
    const char* m_msg;
    struct mg_http_message m_http_msg;
};