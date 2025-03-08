#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiSearchLog(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void searchLogResponSuccess(char* wbuf,int wbuf_sz);
void searchLogResponFailed(char* wbuf,int wbuf_sz);
