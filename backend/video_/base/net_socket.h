#pragma once
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

using EpollInCbType=void (*)();
using EpollOutCbType=void (*)();
class NetSocket{
    //该类只负责网络套接字的初始化，包装一个网络套接字
public:
    NetSocket();
    ~NetSocket();
    void setEpollInCb();
    void setEpollOutCb();
    EpollInCbType getEpollInCb();
    EpollOutCbType getEpollOutCb();
    int getSockFd();
private:
    int m_fd;
    void (* m_epoll_in_cb)();
    void (* m_epoll_out_cb)();
};