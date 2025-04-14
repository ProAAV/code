
#include"api_analysis_messages.h"
#include<iostream>
#include"../base/common.h"
#include<json/json.h>

void apiAnalysisMessages(void* args){
    std::cout<<"enter apiAnalysisMessages\n";
    ApiFuncArgs* ags=(ApiFuncArgs*)args;
    char* wbuf=ags->wbuf;
    int wbuf_sz=ags->wbuf_sz;
    struct mg_http_message hm=ags->hm;
    ConfRead conf_reader=*ags->conf_reader;

    std::string method(hm.method.buf,hm.method.len);
    if(method!="GET"){
        std::cout<<"apiAnalysisMessages error ,the method is not GET\n";
        return ;
    }
    char par_md5_value[36];
    if(mg_http_get_var(&hm.query,"file_md5",par_md5_value,sizeof(par_md5_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        std::cout<<"1111:"<<par_md5_value<<"\n";
        analysisMessagesResponFailed(wbuf,wbuf_sz);
        return;
    }
    
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select message from aav_video_descrpt where file_md5='%s'",par_md5_value);

    std::cout<<"rrrrrrrrrrrrrrrrr:"<<query_<<'\n';

    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(res==nullptr){
        analysisMessagesResponFailed(wbuf,wbuf_sz);
        return;
    }
    analysisMessagesResponSuccess(wbuf,wbuf_sz,res);
    return;
    

}

void analysisMessagesResponSuccess(char* wbuf,int wbuf_sz,MYSQL_RES* res){
    std::cout<<"enter analysisMessagesResponSuccess\n";
    int row_num=mysql_num_rows(res);
    if(row_num!=1){
        std::cout<<"error with analysisMessagesResponSuccess\n";
        mysql_free_result(res);
        return;
    }
    std::cout<<"row_num:"<<row_num<<'\n';
    int field_num=mysql_num_fields(res);
    MYSQL_FIELD* fields=mysql_fetch_fields(res);
    Json::Value root;
    root["status"]="0";
    MYSQL_ROW row=mysql_fetch_row(res);
    for(int j=0;j<field_num;j++){
        root[fields[j].name]=row[j];
    }
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
void analysisMessagesResponFailed(char* wbuf,int wbuf_sz){

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