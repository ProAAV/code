#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiSearchLog(void* args);
void searchLogResponSuccess(char* wbuf,int wbuf_sz);
void searchLogResponFailed(char* wbuf,int wbuf_sz);
