#pragma once

extern "C"{
    #include"mongoose.h"
}
class HttpServer{
public:
    HttpServer();
    ~HttpServer();
    void http_parse_(const char* msg);

private:

};