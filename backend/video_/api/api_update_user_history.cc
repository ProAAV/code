#include"api_update_user_history.h"
#include<iostream>
#include"../base/common.h"
#include<json/json.h>

void UpdateUserHistoryResponSuccess(char* wbuf,int wbuf_sz){
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
void UpdateUserHistoryResponFailed(char* wbuf,int wbuf_sz){
    Json::Value root;
    root["status"]="1";
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

void apiHandleUpdateUserHistory(void* args){
    std::cout<<"enter apiHandleUpdateUserHistory\n";
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;

    std::string method(hm.method.buf,hm.method.len);
    if(method!="POST"){
        std::cout<<"apiHandleUpdateUserHistory error ,the method is not POST\n";
        return ;
    }
    struct mg_str body=hm.body;
    struct mg_str* content_type=mg_http_get_header(&hm,"content-type");
    std::string type(content_type->buf,content_type->len);
    std::string body_data{body.buf,body.len};
    std::cout<<"content_type:"<<type<<'\n';
    if(type!="application/json"){
        std::cout<<"register has no json data\n";
        UpdateUserHistoryResponFailed(wbuf,wbuf_sz);
        return;
    }
    Json::Value root;

    Json::CharReaderBuilder builder;

    std::string errors;
    std::istringstream input_string_stream{body_data};
    if(!Json::parseFromStream(builder,input_string_stream,&root,&errors)){
        std::cout<<"json parser failed:"<<errors<<'\n';
        UpdateUserHistoryResponFailed(wbuf,wbuf_sz);
        return;
    }
    std::cout<<"data:"<<root<<"\n";
    std::string username=root["username"].asString();
    std::string file_md5=root["file_md5"].asString();
    std::string progress_data=root["progress_data"].asString();

    MysqlConn sql_conn{};
    char query_[256];
    //在执行插入操作前先检查有无同账号
    sprintf(query_,"select id from `aav_user_history_view` where username='%s' and file_md5='%s'",username.c_str(),file_md5.c_str());
    std::cout<<"sql:"<<query_<<'\n';
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(!mysql_num_rows(res)){
        //没有这种数据就插入一条
        sprintf(query_,"insert into `aav_user_history_view` (username,file_md5,view_cnt,date_time,progress_data) value ('%s','%s','%d',now(),'%s')",username.c_str(),file_md5.c_str(),1,progress_data.c_str());
        res=sql_conn.mysqlQuery(query_);
        mysql_free_result(res);
        UpdateUserHistoryResponSuccess(wbuf,wbuf_sz);
        return;
    }
    //有记录就直接更新
    sprintf(query_,"update `aav_user_history_view` set view_cnt=view_cnt+1,date_time=now(),progress_data='%s' where username='%s' and file_md5='%s'",progress_data.c_str(),username.c_str(),file_md5.c_str());
    res=sql_conn.mysqlQuery(query_);
    mysql_free_result(res);
    UpdateUserHistoryResponSuccess(wbuf,wbuf_sz);
    return;
}