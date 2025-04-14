#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
void apiAnalysisMessages(void* args);
void analysisMessagesResponSuccess(char* wbuf,int wbuf_sz,MYSQL_RES* res);

void analysisMessagesResponFailed(char* wbuf,int wbuf_sz);