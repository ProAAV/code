#include"api_search_log.h"
#include<iostream>
#include<json/json.h>
#include"../base/common.h"
void apiSearchLog(void* args){
    std::cout<<"enter apiSearchLog\n";
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;

    std::string method(hm.method.buf,hm.method.len);
    if(method!="POST"){
        std::cout<<"apiSearchLog error ,the method is not POST\n";
        return ;
    }
    struct mg_str body=hm.body;
    struct mg_str* content_type=mg_http_get_header(&hm,"content-type");
    std::string type(content_type->buf,content_type->len);
    std::string body_data{body.buf,body.len};
    std::cout<<"content_type:"<<type<<'\n';
    if(type!="application/json"){
        std::cout<<"login has no json data\n";
        searchLogResponFailed(wbuf,wbuf_sz);
        return;
    }
    //用来接收json格式的数据
    Json::Value root;

    Json::CharReaderBuilder builder;

    std::string errors;
    std::istringstream input_string_stream{body_data};
    if(!Json::parseFromStream(builder,input_string_stream,&root,&errors)){
        std::cout<<"json parser failed\n";
        searchLogResponFailed(wbuf,wbuf_sz);
        return;
    }
    std::cout<<"data:"<<root<<"\n";
    std::string username=root["username"].asString();
    std::string search_key=root["search_key"].asString();
    if(username==""||search_key==""){
        std::cout<<"uname or passwd is null\n";
        searchLogResponFailed(wbuf,wbuf_sz);
        return;
    }
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"insert into `aav_user_search_log` (username,date_time,search_key) values ('%s',now(),'%s')",username.c_str(),search_key.c_str());

    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    
    searchLogResponSuccess(wbuf,wbuf_sz);
    return;
}
void searchLogResponSuccess(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]="0";
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
}
void searchLogResponFailed(char* wbuf,int wbuf_sz){
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