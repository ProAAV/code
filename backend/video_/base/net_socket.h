#pragma once
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

using EpollInCbType=void (*)(int);
using EpollOutCbType=void (*)(int);
class NetSocket{
    //该类只负责网络套接字的初始化，包装一个网络套接字
public:
    NetSocket();
    ~NetSocket();
    void setEpollInCb(EpollInCbType func_ptr);
    void setEpollOutCb(EpollOutCbType func_ptr);
    EpollInCbType getEpollInCb();
    EpollOutCbType getEpollOutCb();
    int getSockFd();
private:
    int m_fd;
    void (* m_epoll_in_cb)(int fd);
    void (* m_epoll_out_cb)(int fd);
};