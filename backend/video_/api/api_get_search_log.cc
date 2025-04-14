#include"api_get_search_log.h"
#include<iostream>
#include"../base/common.h"
#include<json/json.h>
void apiGetSearchLog(void* args){
    //整个流程是客户端发送search_key过来，服务端模糊匹配到这些关键词从数据库中取出返回显示在qt的completer中自动补全
    std::cout<<"enter apiGetSearchLog\n";
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;

    std::string method(hm.method.buf,hm.method.len);
    if(method!="GET"){
        std::cout<<"apiGetSearchLog error ,the method is not POST\n";
        return ;
    }
    char par_username_value[32];
    if(mg_http_get_var(&hm.query,"username",par_username_value,sizeof(par_username_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        std::cout<<"1111:"<<par_username_value<<"\n";
        getSearchLogResponFailed(wbuf,wbuf_sz);
        return;
    }
    char par_key_value[32];
    if(mg_http_get_var(&hm.query,"key",par_key_value,sizeof(par_key_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        std::cout<<"1111:"<<par_key_value<<"\n";
        getSearchLogResponFailed(wbuf,wbuf_sz);
        return;
    }
    
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select A.*,B.file_title from aav_file_info as A inner join aav_user_file as B on A.file_md5=B.file_md5 where B.file_title like '%%%s%%' limit 10",par_key_value);

    std::cout<<"rrrrrrrrrrrrrrrrr:"<<query_<<'\n';

    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(res==nullptr){
        getSearchLogResponFailed(wbuf,wbuf_sz);
        return;
    }
    getSearchLogResponSuccess(wbuf,wbuf_sz,res);
    return;
}
void getSearchLogResponSuccess(char* wbuf,int wbuf_sz,MYSQL_RES* res){
    std::cout<<"enter getSearchLogResponSuccess\n";
    int row_num=mysql_num_rows(res);
    std::cout<<"row_num:"<<row_num<<'\n';
    int field_num=mysql_num_fields(res);
    MYSQL_FIELD* fields=mysql_fetch_fields(res);
    Json::Value root;
    root["status"]=0;

    for(int i=0;i<row_num;i++){
        //创建一个子对象
        Json::Value jarr;
        MYSQL_ROW row=mysql_fetch_row(res);
        for(int j=0;j<field_num;j++){
            jarr[fields[j].name]=row[j];
        }
        //添加子对象
        root[std::to_string(i)]=jarr;
    }
    root["search_log_cnt"]=row_num;
    std::cout<<root<<'\n';

    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, root);
    std::string response="HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(jsonStr.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            jsonStr;
    if(jsonStr.size()>wbuf_sz){
        std::cout<<"size is too small\n";
        return;
    }
    memcpy(wbuf,response.c_str(),response.size());
    std::cout<<"response success\n";
}
void getSearchLogResponFailed(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]="1";
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, root);
    std::string response="HTTP/1.1 401 Unauthorized\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(jsonStr.size()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            jsonStr;
    if(jsonStr.size()>wbuf_sz){
        std::cout<<"size is too small\n";
        return;
    }
    memcpy(wbuf,response.c_str(),response.size());
}