#include"api_register.h"
#include<iostream>
#include"../base/common.h"
#include<json/json.h>
void apiRegister(void* args){
    std::cout<<"enter apiRegister\n";
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;

    std::string method(hm.method.buf,hm.method.len);
    if(method!="POST"){
        std::cout<<"apiRegister error ,the method is not POST\n";
        return ;
    }
    struct mg_str body=hm.body;
    struct mg_str* content_type=mg_http_get_header(&hm,"content-type");
    std::string type(content_type->buf,content_type->len);
    std::string body_data{body.buf,body.len};
    std::cout<<"content_type:"<<type<<'\n';
    if(type!="application/json"){
        std::cout<<"register has no json data\n";
        registerResponError(wbuf,wbuf_sz);
        return;
    }
    Json::Value root;

    Json::CharReaderBuilder builder;

    std::string errors;
    std::istringstream input_string_stream{body_data};
    if(!Json::parseFromStream(builder,input_string_stream,&root,&errors)){
        std::cout<<"json parser failed:"<<errors<<'\n';
        registerResponError(wbuf,wbuf_sz);
        return;
    }
    std::cout<<"data:"<<root<<"\n";
    std::string username=root["account"].asString();
    std::string password=root["password"].asString();
    std::string nickname=root["nickname"].asString();
    

    if(username==""||password==""||nickname==""){
        std::cout<<"uname or passwd or nickname  is null\n";
        registerResponError(wbuf,wbuf_sz);
        return;
    }
    MysqlConn sql_conn{};
    char query_[256];
    //在执行插入操作前先检查有无同账号
    sprintf(query_,"select * from `aav_user_info` where username='%s'",username.c_str());
    std::cout<<"sql:"<<query_<<'\n';
    std::cout<<"name:"<<username<<'\n';
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(mysql_num_rows(res)){
        std::cout<<"aav_user_info has exists a row\n";
        registerResponFailed(wbuf,wbuf_sz);
        return;
    }
    sprintf(query_,"insert into `aav_user_info` (nickname,username,userpassword) value ('%s','%s',SHA2('%s', 256))",nickname.c_str(),username.c_str(),password.c_str());
    res=sql_conn.mysqlQuery(query_);

    registerResponSuccess(wbuf,wbuf_sz);
    return;
}
void registerResponSuccess(char* wbuf,int wbuf_sz){
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
void registerResponFailed(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]="1";
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, root);
    std::string response="HTTP/1.1 409 Conflict\r\n"
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
void registerResponError(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]="2";
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, root);
    std::string response="HTTP/1.1 500 Internal Server Error\r\n"
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