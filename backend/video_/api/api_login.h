#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiLogin(void* args);
void loginResponSuccess(char* wbuf,int wbuf_sz);
void loginResponFailed(char* wbuf,int wbuf_sz);
void loginResponError(char* wbuf,int wbuf_sz);