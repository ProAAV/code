#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiRegister(void* args);
void registerResponSuccess(char* wbuf,int wbuf_sz);
void registerResponFailed(char* wbuf,int wbuf_sz);
void registerResponError(char* wbuf,int wbuf_sz);