#include"net_socket.h"
#include<unistd.h>
#include<iostream>
NetSocket::NetSocket(int fd):m_fd(fd){
    //分配读写缓冲区
    m_rbuf=new char[RBUF_SIZE];
    m_wbuf=new char[WBUF_SIZE];
    m_epoll_in_cb=nullptr;
    m_epoll_out_cb=nullptr;
}
NetSocket::~NetSocket(){
    close(m_fd);
}
void NetSocket::setEpollInCb(EpollInCbType func_ptr){
    m_epoll_in_cb=func_ptr;
}
void NetSocket::setEpollOutCb(EpollOutCbType func_ptr){
    m_epoll_out_cb=func_ptr;
}
EpollInCbType NetSocket::getEpollInCb(){
    if(m_epoll_in_cb==nullptr){
        std::cout<<"epoll_in is null\n";
        
    }
    return m_epoll_in_cb;
}
EpollOutCbType NetSocket::getEpollOutCb(){
    if(m_epoll_out_cb==nullptr){
        std::cout<<"epoll_out is null\n";
        
    }
    return m_epoll_out_cb;
}
int NetSocket::getSockFd(){
    return m_fd;
}
char* NetSocket::getReadBuffer(){
    return m_rbuf;
}
char* NetSocket::getWriteBuffer(){
    return m_wbuf;
}