#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiHandleUpdateUserHistory(void* args);
void UpdateUserHistoryResponSuccess(char* wbuf,int wbuf_sz);
void UpdateUserHistoryResponFailed(char* wbuf,int wbuf_sz);