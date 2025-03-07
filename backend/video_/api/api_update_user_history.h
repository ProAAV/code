#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiHandleUpdateUserHistory(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void UpdateUserHistoryResponSuccess(char* wbuf,int wbuf_sz);
void UpdateUserHistoryResponFailed(char* wbuf,int wbuf_sz);