#include"api_files_list.h"
#include"../base/common.h"
#include"../base/mysqlconn.h"
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
        return;
    }
    char par_strategy_value[16];
    if(mg_http_get_var(&hm.query,"strategy",par_strategy_value,sizeof(par_strategy_value))<=0){
        std::cout<<"mg_http_get_var error\n";
        return;
    }
    //策略为random时调用的数据组织函数
    std::cout<<"hhhhhhh\n";
    if(strcmp(par_strategy_value,"random")==0){
        isNumber(std::string(par_files_value));
        std::cout<<"kkkkkkkkkkk\n";
        constructFileRandom(std::stoi(par_files_value));
    }
    
}
void constructFileRandom(int file_cnt){
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
    if(std::stoi(count)<file_cnt){
        std::cout<<"files is not enough\n";
        mysql_free_result(res);
        return;
    }
    sprintf(query_,"select * from aav_file_info where id%%2 = floor(rand()*2) limit 10");
    res=sql_conn.mysqlQuery(query_);
    std::cout<<"000000\n";
    
    mysql_free_result(res);
}