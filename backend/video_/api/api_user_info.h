#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiUserInfo(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void UserInfoResponseSuccess(MYSQL_RES* res,char* wbuf,int wbuf_sz);
void UserInfoResponseFailed(char* wbuf,int wbuf_sz);