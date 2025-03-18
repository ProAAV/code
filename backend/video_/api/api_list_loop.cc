#include"api_list_loop.h"
#include<iostream>
#include<json/json.h>
void apiListLoop(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader){
    std::cout<<"enter list loop\n";
    std::string method(hm.method.buf,hm.method.len);
    if(method!="POST"){
        std::cout<<"apiListLoop error ,the method is not POST\n";
        return ;
    }
    struct mg_str body=hm.body;
    struct mg_str* content_type=mg_http_get_header(&hm,"content-type");
    std::string type(content_type->buf,content_type->len);
    std::string body_data{body.buf,body.len};
    std::cout<<"content_type:"<<type<<'\n';
    if(type!="application/json"){
        std::cout<<"listloop has no json data\n";
        listLoopResponFailed(wbuf,wbuf_sz);
        return;
    }
    //用来接收json格式的数据
    Json::Value root;

    Json::CharReaderBuilder builder;

    std::string errors;
    std::istringstream input_string_stream{body_data};
    if(!Json::parseFromStream(builder,input_string_stream,&root,&errors)){
        std::cout<<"json parser failed\n";
        listLoopResponFailed(wbuf,wbuf_sz);
        return;
    }
    std::cout<<"data:"<<root<<"\n";
    std::string username=root["username"].asString();
    std::string file_md5=root["file_md5"].asString();
    std::string file_type=root["file_type"].asString();
    if(username==""||file_md5==""||file_type==""){
        std::cout<<"uname or file_md5 or file_type is null\n";
        listLoopResponFailed(wbuf,wbuf_sz);
        return;
    }
    MysqlConn sql_conn{};
    char query_[1024];
    sprintf(query_,"select file_feature_1,file_feature_2,file_feature_3,file_feature_4,file_feature_5 from `aav_file_info` where file_md5='%s'",file_md5.c_str());
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(!mysql_num_rows(res)){
        //用户表中找不到用户信息，返回1
        listLoopResponFailed(wbuf,wbuf_sz);
        return;
    }
    int field_num=mysql_num_fields(res);
    if(field_num!=5){
        std::cout<<"field num!=5\n";
        return;
    }
    MYSQL_ROW row=mysql_fetch_row(res);
    sprintf(query_,"SELECT aav_file_info.*, ( CASE WHEN file_feature_1 = '%s' THEN 1 ELSE 0 END +CASE WHEN file_feature_2 = '%s' THEN 1 ELSE 0 END +CASE WHEN file_feature_3 = '%s' THEN 1 ELSE 0 END +CASE WHEN file_feature_4 = '%s' THEN 1 ELSE 0 END +CASE WHEN file_feature_5 = '%s' THEN 1 ELSE 0 END ) AS similarity_score, aav_user_file.file_title,aav_user_file.username FROM aav_file_info inner join aav_user_file on aav_file_info.file_md5 = aav_user_file.file_md5 WHERE aav_file_info.file_md5 != '%s' AND file_type='%s' ORDER BY similarity_score DESC LIMIT 6",row[0],row[1],row[2],row[3],row[4],file_md5.c_str(),file_type.c_str());
    std::cout<<"loop list query:"<<query_<<'\n';
    res=sql_conn.mysqlQuery(query_);
    listLoopResponSuccess(wbuf,wbuf_sz,res);
    return;
}
void listLoopResponSuccess(char* wbuf,int wbuf_sz,MYSQL_RES* res){
    int row_num=mysql_num_rows(res);
    std::cout<<"row_num:"<<row_num<<'\n';
    int field_num=mysql_num_fields(res);
    MYSQL_FIELD* fields=mysql_fetch_fields(res);
    Json::Value root;
    root["status"]="0";

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
    root["file_info_cnt"]=row_num;
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
void listLoopResponFailed(char* wbuf,int wbuf_sz){

}
void listLoopResponError(char* wbuf,int wbuf_sz){

}