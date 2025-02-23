#pragma once
#include<string>
#define HOST "192.168.208.128"
#define USER "root"
#define PASSWD "2028915986My!"
#define DB "aav_server"
#define PORT 3306
extern "C"{
    #include<mysql/mysql.h>
}
class MysqlConn{
public:
    MysqlConn();
    ~MysqlConn();
    MYSQL_RES* mysqlQuery(const std::string& query);
private:
    MYSQL* m_conn;
    std::string m_host;
    std::string m_user;
    std::string m_passwd;
    std::string m_db;
    int m_port;
    
};
