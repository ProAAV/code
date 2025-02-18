#pragma once
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
class EventLoop;
using EpollInCbType=void (*)(int fd,EventLoop& evloop);
using EpollOutCbType=void (*)(int fd,EventLoop& evloop);
#define WBUF_SIZE 4096
#define RBUF_SIZE 4096
class NetSocket{
    //该类只负责网络套接字的初始化，包装一个网络套接字
public:
    NetSocket(int fd);
    ~NetSocket();
    void setEpollInCb(EpollInCbType func_ptr);
    void setEpollOutCb(EpollOutCbType func_ptr);
    EpollInCbType getEpollInCb();
    EpollOutCbType getEpollOutCb();
    char* getReadBuffer();
    char* getWriteBuffer();
    int getSockFd();
    int getReadBufferSize();
    int getWriteBufferSize();
private:
    int m_fd;
    void (* m_epoll_in_cb)(int fd,EventLoop& evloop);
    void (* m_epoll_out_cb)(int fd,EventLoop& evloop);
    char* m_rbuf;
    char* m_wbuf;
};