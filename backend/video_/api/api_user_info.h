#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiUserInfo(void* args);
void UserInfoResponseSuccess(MYSQL_RES* res,char* wbuf,int wbuf_sz);
void UserInfoResponseFailed(char* wbuf,int wbuf_sz);