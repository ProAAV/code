#include"api_login.h"
#include<iostream>
#include<json/json.h>
#include"../base/common.h"
//定义回发状态码，0表示登录成功，1表示账号或密码错误，2表示服务器出错

void apiLogin(void* args){
    std::cout<<"enter apiLogin\n";
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;
    std::string method(hm.method.buf,hm.method.len);
    if(method!="POST"){
        std::cout<<"apiFilesList error ,the method is not GET\n";
        return ;
    }
    struct mg_str body=hm.body;
    struct mg_str* content_type=mg_http_get_header(&hm,"content-type");
    std::string type(content_type->buf,content_type->len);
    std::string body_data{body.buf,body.len};
    std::cout<<"content_type:"<<type<<'\n';
    if(type!="application/json"){
        std::cout<<"login has no json data\n";
        loginResponError(wbuf,wbuf_sz);
        return;
    }
    //用来接收json格式的数据
    Json::Value root;

    Json::CharReaderBuilder builder;

    std::string errors;
    std::istringstream input_string_stream{body_data};
    if(!Json::parseFromStream(builder,input_string_stream,&root,&errors)){
        std::cout<<"json parser failed\n";
        loginResponError(wbuf,wbuf_sz);
        return;
    }
    std::cout<<"data:"<<root<<"\n";
    std::string username=root["account"].asString();
    std::string password=root["password"].asString();
    if(username==""||password==""){
        std::cout<<"uname or passwd is null\n";
        loginResponError(wbuf,wbuf_sz);
        return;
    }
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select * from `aav_user_info` where username='%s' and userpassword=SHA2('%s', 256)",username.c_str(),password.c_str());
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(!res){
        loginResponFailed(wbuf,wbuf_sz);
        std::cout<<"query error\n";
        return;
    }
    if(!mysql_num_rows(res)){
        //用户表中找不到用户信息，返回1
        loginResponFailed(wbuf,wbuf_sz);
        return;
    }
    loginResponSuccess(wbuf,wbuf_sz);
    return;
}
void loginResponSuccess(char* wbuf,int wbuf_sz){
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
void loginResponFailed(char* wbuf,int wbuf_sz){
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
void loginResponError(char* wbuf,int wbuf_sz){
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