#include"api_search.h"
#include"../base/common.h"
void apiSearch(void* args){
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;
}
void searchResponSuccess(char* wbuf,int wbuf_sz){

}
void searchResponFailed(char* wbuf,int wbuf_sz){
    
}