#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiSearch(void* agrs);
void searchResponSuccess(char* wbuf,int wbuf_sz);
void searchResponFailed(char* wbuf,int wbuf_sz);
