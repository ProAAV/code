#pragma once
#include"net_socket.h"
#include<unordered_map>
#include<sys/epoll.h>
#include<vector>
#include"config_read.h"
#define EPOLL_EVENTS_MAX 64
//套接字的accept行为
void acceptCallback(int fd,EventLoop& evloop);
//套接字的recv行为
void recvCallback(int fd,EventLoop& evloop);
//套接字的send行为
void sendCallback(int fd,EventLoop& evloop);


class EventLoop{
public:
    friend void acceptCallback(int fd,EventLoop& evloop);
    friend void recvCallback(int fd,EventLoop& evloop);
    friend void sendCallback(int fd,EventLoop& evloop);
    EventLoop(ConfRead& conf_reader);
    ~EventLoop();
    
    //创建服务端网络套接字
    void serverSocketsFdCreate();
    //服务端网络套接字绑定ip地址
    void bindAddress(int sfd,int port);
    //服务端网络套接字监听数据流
    void listenEvent(int sfd);
    //进入网络收发事件循环
    void enterLoop(EventLoop& evloop);
    //启动事件循环
    void eventLoopStart(EventLoop& evloop);

    //创建epoll文件描述符
    void epollCreate();
    //为将要添加到epoll中的文件描述注册事件
    struct epoll_event* regisEvent(int fd,int ev);
    //添加文件描述符进入epoll中
    void epollAddFd(int fd,int ev);
    //改变文件描述符的epoll事件
    void epollEventMod(int fd,int ev);
    

private:
    std::vector<int> separate_ports(const std::string& ports,int nb_port);

    int m_epollfd;
    struct epoll_event m_epoll_events[EPOLL_EVENTS_MAX];
    //fd通过map来寻找到对应的NetSocket对象
    std::unordered_map<int,NetSocket*> m_map;
    ConfRead m_conf_reader;
};
