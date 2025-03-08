#include"api_files_list.h"
#include"../base/common.h"
#include<string.h>
#include<json/json.h>
#include<iostream>
void apiFilesList(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader){
    //先解析请求行所请求的参数
    std::cout<<"enter apiFilesList\n";
    std::string method(hm.method.buf,hm.method.len);
    if(method!="GET"){
        std::cout<<"apiFilesList error ,the method is not GET\n";
        return ;
    }
    char par_files_value[8];
    if(mg_http_get_var(&hm.query,"files",par_files_value,sizeof(par_files_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        std::cout<<"3424:"<<par_files_value<<"\n";
        return;
    }
    char par_strategy_value[16];
    if(mg_http_get_var(&hm.query,"strategy",par_strategy_value,sizeof(par_strategy_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        std::cout<<"1111:"<<par_strategy_value<<"\n";
        return;
    }
    std::cout<<"par_strategy_value:"<<par_strategy_value<<'\n';
    char par_username_value[32];
    if(mg_http_get_var(&hm.query,"username",par_username_value,sizeof(par_username_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        std::cout<<"1111:"<<par_username_value<<"\n";
        return;
    }
    std::cout<<"par_search_key_value start\n";
    char par_search_key_value[32];
    if(mg_http_get_var(&hm.query,"key",par_search_key_value,sizeof(par_search_key_value))<=0){
        goto next;
    }std::cout<<"par_search_key_value out\n";
    //策略为random时调用的数据组织函数
next:
    std::cout<<"hhhhhhh\n";
    char sql[256];
    isNumber(std::string(par_files_value));    
    int file_cnt=std::stoi(par_files_value);
    if(strcmp(par_strategy_value,"random")==0){
        constructFileListsInfoRandom(file_cnt,wbuf,wbuf_sz);
    }
    else if(strcmp(par_strategy_value,"userowned")==0){
        std::cout<<"enter userowned\n";
        constructFileListsInfoUserowned(file_cnt,wbuf,wbuf_sz,par_username_value);
    }
    else if(strcmp(par_strategy_value,"userhistory")==0){
        std::cout<<"enter userhistory\n";
        constructFileListsInfoUserhistory(file_cnt,wbuf,wbuf_sz,par_username_value);
    }
    else if(strcmp(par_strategy_value,"search")==0){
        std::cout<<"enter search\n";
        constructFileListsInfoSearch(file_cnt,wbuf,wbuf_sz,par_search_key_value);
    }
    else{
        std::cout<<"par_strategy_value is invalid\n";
        return;
    }
    
}
void constructFileListsInfoUserowned(int file_cnt,char* wbuf,int wbuf_sz,char* username){
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select * from `aav_user_file` A  join `aav_file_info` B on A.file_md5 = B.file_md5 where A.username='%s'",username);
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(res==nullptr){
        std::cout<<"error with constructFileListsInfoUserowned\n";
        return ;
    }
    std::cout<<"enter constructFileListsInfoUserowned\n";
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    mysql_free_result(res);
}
void constructFileListsInfoUserhistory(int file_cnt,char* wbuf,int wbuf_sz,char* username){
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select B.*,A.date_time,A.progress_data from  aav_user_history_view as A inner join aav_file_info as B on A.file_md5=B.file_md5 where A.username='%s' order by A.date_time desc limit 6 ",username);
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    
    if(mysql_num_rows(res)<=0){
        fileListResponseFailed(res,wbuf,wbuf_sz);
        mysql_free_result(res);
        return;
    }
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    mysql_free_result(res);
}
void constructFileListsInfoSearch(int file_cnt,char* wbuf,int wbuf_sz,char* search_key){
    std::cout<<"enter constructFileListsInfoSearch\n";
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select * from `aav_file_info` where file_title like '%%%s%%'  limit 6 ",search_key);
    std::cout<<"constructFileListsInfoSearch sql:"<<query_<<'\n';
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    
    if(mysql_num_rows(res)<=0){
        fileListResponseFailed(res,wbuf,wbuf_sz);
        mysql_free_result(res);
        return;
    }
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    mysql_free_result(res);
    std::cout<<"out constructFileListsInfoSearch\n";
}
void constructFileListsInfoRandom(int file_cnt,char* wbuf,int wbuf_sz){
    //首先就是去数据库中查找这个个数是否足够，如果够了则继续组织数据
    std::cout<<"pppppppppppp\n";
    MysqlConn sql_conn{};
    char query_[256];
    sprintf(query_,"select count(*) from `aav_file_info`");
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    MYSQL_ROW row=mysql_fetch_row(res);
    char* count=new char[10];
    count=row[0];
    isNumber(std::string(count));
    //如果数据库中没有足够的文件数据，那么直接返回
    /*if(std::stoi(count)<file_cnt){
        std::cout<<"files is not enough\n";
        mysql_free_result(res);
        return;
    }*/

    sprintf(query_,"select B.*,A.date_time,A.progress_data from  aav_user_history_view as A inner join aav_file_info as B on A.file_md5=B.file_md5 where B.id%%2 = floor(rand()*2) limit 10");
    res=sql_conn.mysqlQuery(query_);
    std::cout<<"000000\n";
    //组织回发数据
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    mysql_free_result(res);
}
void fileListResponseSuccess(MYSQL_RES* res,char* wbuf,int wbuf_sz){
    //现在的情况下status=0表示成功
    //先读取数据库返回的记录
    std::cout<<"enter fileListResponseSuccess\n";
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

void fileListResponseFailed(MYSQL_RES* res,char* wbuf,int wbuf_sz){

}