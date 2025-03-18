#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"

void apiListLoop(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void listLoopResponSuccess(char* wbuf,int wbuf_sz,MYSQL_RES* res);
void listLoopResponFailed(char* wbuf,int wbuf_sz);
void listLoopResponError(char* wbuf,int wbuf_sz);