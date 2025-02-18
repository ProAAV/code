#pragma once
#include<string>
#include"../base/file_info.h"

extern "C"{
    #include"../base/mongoose.h"
}

void apiUpload(char* wbuf,int wbuf_sz,struct mg_http_message hm);
void uploadHandleFileTLS(CfileInfo& file_info);