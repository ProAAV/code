#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiGetSearchLog(void* args);
void getSearchLogResponSuccess(char* wbuf,int wbuf_sz,MYSQL_RES* res);
void getSearchLogResponFailed(char* wbuf,int wbuf_sz);