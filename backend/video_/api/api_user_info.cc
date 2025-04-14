#include"api_user_info.h"
#include<iostream>
#include"../base/common.h"
#include<json/json.h>

void apiUserInfo(void* args){
    std::cout<<"enter  apiUserInfo\n"<<'\n';
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;

    std::string method(hm.method.buf,hm.method.len);
    if(method!="GET"){
        std::cout<<"apiUserInfo error ,the method is not GET\n";
        UserInfoResponseFailed(wbuf,wbuf_sz);
        return ;
    }
    std::cout<<"getget\n";
    char par_username_value[32];
    if(mg_http_get_var(&hm.query,"username",par_username_value,sizeof(par_username_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        std::cout<<"1111:"<<par_username_value<<"\n";
        UserInfoResponseFailed(wbuf,wbuf_sz);
        return;
    }
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select nickname,username from `aav_user_info` where username='%s'",par_username_value);
    std::cout<<"userinfo query:"<<query_<<'\n';
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    std::cout<<"userinfo mysql_num_rows(res):"<<mysql_num_rows(res)<<'\n';
    if(mysql_num_rows(res))UserInfoResponseSuccess(res,wbuf,wbuf_sz);
    else UserInfoResponseFailed(wbuf,wbuf_sz);
    mysql_free_result(res);
}

void UserInfoResponseSuccess(MYSQL_RES* res,char* wbuf,int wbuf_sz){
    int row_num=mysql_num_rows(res);
    std::cout<<"row_num:"<<row_num<<'\n';
    int field_num=mysql_num_fields(res);
    MYSQL_FIELD* fields=mysql_fetch_fields(res);
    Json::Value root;
    root["status"]=0;

    MYSQL_ROW row=mysql_fetch_row(res);
    for(int j=0;j<field_num;j++){

        root[fields[j].name]=row[j];
    }

    std::cout<<"userinfo:"<<root<<'\n';

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
void UserInfoResponseFailed(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]=1;
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