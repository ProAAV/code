#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiGetSearchLog(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void getSearchLogResponSuccess(char* wbuf,int wbuf_sz,MYSQL_RES* res);
void getSearchLogResponFailed(char* wbuf,int wbuf_sz);