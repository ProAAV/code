#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiRegister(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void registerResponSuccess(char* wbuf,int wbuf_sz);
void registerResponFailed(char* wbuf,int wbuf_sz);
void registerResponError(char* wbuf,int wbuf_sz);