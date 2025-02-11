#pragma once
#include"net_socket.h"
#include<unordered_map>
#include<sys/epoll.h>
//套接字的accept行为
void acceptCallback(int fd);
//套接字的recv行为
void recvCallback(int fd);
//套接字的send行为
void sendCallback(int fd);
#define EPOLL_EVENTS_MAX 64
class EventLoop{
public:
    EventLoop();
    ~EventLoop();
    //创建服务端网络套接字
    void serverSocketFdCreate();
    //服务端网络套接字绑定ip地址
    void bindAddress();
    //服务端网络套接字监听数据流
    void listenEvent();
    //进入网络收发事件循环
    void enterLoop();
    //启动事件循环
    void eventLoopStart();

    //创建epoll文件描述符
    void epollCreate();
    //为将要添加到epoll中的文件描述注册事件
    struct epoll_event* regisEvent(int fd,int ev);
    //添加文件描述符进入epoll中
    void epollAddFd(int fd);


    

private:
    NetSocket* m_server_socket;
    int m_epollfd;
    struct epoll_event m_epoll_events[EPOLL_EVENTS_MAX];
    //fd通过map来寻找到对应的NetSocket对象
    std::unordered_map<int,NetSocket*> m_map;
};
