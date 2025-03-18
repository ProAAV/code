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
    std::string query(hm.query.buf,hm.query.len);
    std::cout<<"apiFilesList query:"<<query<<'\n';
    char par_offset[8]={0};
    if(query.find("offset",0)!=std::string::npos){
        std::cout<<"find offset\n";
        if(mg_http_get_var(&hm.query,"offset",par_offset,sizeof(par_offset))<=0){
            std::cout<<"mg_http_get_var error\n";
            std::cout<<"3424:"<<par_offset<<"\n";
            return;
        }
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
    char par_search_key_value[32]={0};
    if(query.find("key",0)!=std::string::npos){
        if(mg_http_get_var(&hm.query,"key",par_search_key_value,sizeof(par_search_key_value))<=0){
            std::cout<<"mg_http_get_var error\n";
            std::cout<<"3424:"<<par_search_key_value<<"\n";
            return;
        }
    }
    char par_search_for_audios_and_videos_value[32]={0};
    if(query.find("key_search",0)!=std::string::npos){
        if(mg_http_get_var(&hm.query,"key_search",par_search_for_audios_and_videos_value,sizeof(par_search_for_audios_and_videos_value))<=0){
            std::cout<<"mg_http_get_var error\n";
            std::cout<<"3424:"<<par_search_for_audios_and_videos_value<<"\n";
            return;
        }
    }
    //策略为random时调用的数据组织函数
    std::cout<<"hhhhhhh\n";
    char sql[256];
    isNumber(std::string(par_files_value));    
    int file_cnt=std::stoi(par_files_value);
    if(strcmp(par_strategy_value,"random")==0){
        std::cout<<"enter random\n";
        if(strlen(par_search_key_value)&&strcmp(par_search_key_value,"0")==0){
            //有且关键字参数是"0"，那么说明只组织音频文件
            std::cout<<"enter random 1\n";
            if(strlen(par_search_for_audios_and_videos_value)){
                //说明有key_search这个参数，那么在组织音频文件时要带这个参数
                constructFileListsAudio(file_cnt,wbuf,wbuf_sz,par_search_for_audios_and_videos_value);
            }
            else{
                constructFileListsAudio(file_cnt,wbuf,wbuf_sz);
            }
            
        }
        else if(strcmp(par_search_key_value,"1")==0){
            //只组织视频文件
            if(strlen(par_search_for_audios_and_videos_value)){
                constructFileListsVideo(file_cnt,wbuf,wbuf_sz,par_search_for_audios_and_videos_value);
            }
            else{
                constructFileListsVideo(file_cnt,wbuf,wbuf_sz);
            }

            
        }
        else{
            //混合组织
            std::cout<<"enter random 3\n";
            constructFileListsInfoRandom(file_cnt,wbuf,wbuf_sz,par_offset);
        }
        
    }
    else if(strcmp(par_strategy_value,"userowned")==0){
        std::cout<<"enter userowned\n";
        constructFileListsInfoUserowned(file_cnt,wbuf,wbuf_sz,par_username_value,par_offset);
    }
    else if(strcmp(par_strategy_value,"userhistory")==0){
        std::cout<<"enter userhistory\n";
        constructFileListsInfoUserhistory(file_cnt,wbuf,wbuf_sz,par_username_value,par_offset);
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
void constructFileListsAudio(int file_cnt,char* wbuf,int wbuf_sz,char* key_search){
    MysqlConn sql_conn{};
    char query_[256];
    
    sprintf(query_,"select B.*,A.date_time,A.username,A.file_title from  aav_user_file as A inner join aav_file_info as B on A.file_md5=B.file_md5 where B.id%%2 = floor(rand()*2) and B.file_type=0 and A.file_title like '%%%s%%' limit 4",key_search);
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    std::cout<<"000000\n";
    //组织回发数据
    std::cout<<"constructFileListsAudio------------------------------\n";
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    std::cout<<"out constructFileListsAudio------------------------------\n";

    mysql_free_result(res);
}
void constructFileListsVideo(int file_cnt,char* wbuf,int wbuf_sz,char* key_search){
    MysqlConn sql_conn{};
    char query_[256];
    
    sprintf(query_,"select B.*,A.date_time,A.username,A.file_title from  aav_user_file as A inner join aav_file_info as B on A.file_md5=B.file_md5 where B.id%%2 = floor(rand()*2) and B.file_type=1 and A.file_title like '%%%s%%' limit 4",key_search);
    std::cout<<"constructFileListsVideo search key query:"<<query_<<'\n';
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    std::cout<<"000000\n";
    //组织回发数据
    std::cout<<"constructFileListsAudio------------------------------\n";
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    std::cout<<"out constructFileListsAudio------------------------------\n";

    mysql_free_result(res);
}
void constructFileListsAudio(int file_cnt,char* wbuf,int wbuf_sz){
    std::cout<<"enter constructFileListsAudio\n";
    MysqlConn sql_conn{};
    char query_[256];
    
    sprintf(query_,"select B.*,A.date_time,A.username,A.file_title from  aav_user_file as A inner join aav_file_info as B on A.file_md5=B.file_md5 where B.id%%2 = floor(rand()*2) and B.file_type='0' limit 4");
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    std::cout<<"000000\n";
    //组织回发数据
    std::cout<<"constructFileListsAudio------------------------------\n";
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    std::cout<<"out constructFileListsAudio------------------------------\n";

    mysql_free_result(res);
}
void constructFileListsVideo(int file_cnt,char* wbuf,int wbuf_sz){
    std::cout<<"enter constructFileListsVideo\n";
    MysqlConn sql_conn{};
    char query_[256];
    
    sprintf(query_,"select B.*,A.date_time,A.username,A.file_title from  aav_user_file as A inner join aav_file_info as B on A.file_md5=B.file_md5 where B.id%%2 = floor(rand()*2) and B.file_type='1' limit 4");
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    std::cout<<"000000\n";
    //组织回发数据
    std::cout<<"constructFileListsVideo------------------------------\n";
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    std::cout<<"out constructFileListsVideo------------------------------\n";

    mysql_free_result(res);
}
void constructFileListsInfoUserowned(int file_cnt,char* wbuf,int wbuf_sz,char* username,char* offset){
    std::cout<<"----------------------------\n";
    std::cout<<"enter constructFileListsInfoUserowned\n";
    MysqlConn sql_conn{};
    char query_[256];
    if(strlen(offset)!=0){
        isNumber(offset);
        
        sprintf(query_,"select * from `aav_user_file` A  join `aav_file_info` B on A.file_md5 = B.file_md5 where A.username='%s' limit 4 offset %s",username,offset);
        std::cout<<"sql:"<<query_<<'\n';
    }
    else{
        sprintf(query_,"select * from `aav_user_file` A  join `aav_file_info` B on A.file_md5 = B.file_md5 where A.username='%s' limit 4",username);
        std::cout<<"sql2:"<<query_<<'\n';
    }
    std::cout<<"----------------------------\n";
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    if(res==nullptr){
        std::cout<<"error with constructFileListsInfoUserowned\n";
        return ;
    }
    std::cout<<"enter constructFileListsInfoUserowned\n";
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    mysql_free_result(res);
}
void constructFileListsInfoUserhistory(int file_cnt,char* wbuf,int wbuf_sz,char* username,char* offset){
    MysqlConn sql_conn{};
    char query_[512];
    if(strlen(offset)!=0){
        isNumber(offset);
        sprintf(query_,"select B.*,A.date_time,A.progress_data,C.username,C.file_title from  aav_user_history_view as A inner join aav_file_info as B on A.file_md5=B.file_md5  join aav_user_file as C on C.file_md5=B.file_md5  where A.username='%s' order by A.date_time desc limit 4 offset %s;",username,offset);
    }
    else{
        sprintf(query_,"select B.*,A.date_time,A.progress_data,C.username,C.file_title from  aav_user_history_view as A inner join aav_file_info as B on A.file_md5=B.file_md5  join aav_user_file as C on C.file_md5=B.file_md5  where A.username='%s' order by A.date_time desc limit 4;",username);
    }
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
    //sprintf(query_,"select * from `aav_file_info` where file_title like '%%%s%%'  limit 6 ",search_key);
    sprintf(query_,"select A.*,B.date_time,B.username,B.file_title from aav_file_info as A inner join aav_user_file as B on A.file_md5=B.file_md5 where B.file_title like '%%%s%%'  limit 4;",search_key);

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
void constructFileListsInfoRandom(int file_cnt,char* wbuf,int wbuf_sz,char* offset){
   
    MysqlConn sql_conn{};
    char query_[256];
    if(strlen(offset)!=0){
        std::cout<<"offset is not nullptr\n";
        std::cout<<"offset:"<<offset<<'\n';
        isNumber(offset);
        std::string off{offset};
        int oft=std::stoi(offset);
        sprintf(query_,"select B.*,A.date_time,A.username,A.file_title from  aav_user_file as A inner join aav_file_info as B on A.file_md5=B.file_md5 limit 4 offset %d",oft);
    }
    else{
        sprintf(query_,"select B.*,A.date_time,A.username,A.file_title from  aav_user_file as A inner join aav_file_info as B on A.file_md5=B.file_md5 limit 4");
    }
    MYSQL_RES* res=sql_conn.mysqlQuery(query_);
    std::cout<<"000000\n";
    //组织回发数据
    fileListResponseSuccess(res,wbuf,wbuf_sz);
    mysql_free_result(res);
}
void fileListResponseSuccess(MYSQL_RES* res,char* wbuf,int wbuf_sz){
    //现在的情况下status=0表示成功
    //先读取数据库返回的记录
    //任何回复在写之前一定先清空数组
    memset(wbuf,0,wbuf_sz);
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