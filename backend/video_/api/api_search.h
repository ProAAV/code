#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiSearch(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void searchResponSuccess(char* wbuf,int wbuf_sz);
void searchResponFailed(char* wbuf,int wbuf_sz);
