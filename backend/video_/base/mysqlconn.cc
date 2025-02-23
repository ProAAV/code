#include"mysqlconn.h"
#include<iostream>
MysqlConn::MysqlConn():m_host(HOST),m_user(USER),m_passwd(PASSWD),m_db(DB),m_port(PORT){
    m_conn=mysql_init(nullptr);
    if(!m_conn){
        std::cout<<"mysql conn is failed to init\n";
        return;
    }
    if(!mysql_real_connect(m_conn,m_host.c_str(),m_user.c_str(),m_passwd.c_str(),m_db.c_str(),m_port,nullptr,0)){
        std::cout<<"mysql_real_connect failed\n";
        mysql_close(m_conn);
        return;
    }

}
MysqlConn::~MysqlConn(){
    mysql_close(m_conn);
}
MYSQL_RES* MysqlConn::mysqlQuery(char* query){
    if(mysql_query(m_conn,query)){
        std::cout<<"mysql_query failed\n";
        std::cout<<mysql_error(m_conn)<<"\n";
        mysql_close(m_conn);
        return nullptr;
    }
    return mysql_store_result(m_conn);
}
