#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiLogin(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void loginResponSuccess(char* wbuf,int wbuf_sz);
void loginResponFailed(char* wbuf,int wbuf_sz);
void loginResponError(char* wbuf,int wbuf_sz);