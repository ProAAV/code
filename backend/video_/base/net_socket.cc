#include"net_socket.h"
#include<unistd.h>
NetSocket::NetSocket(){
    //分配一个网络套接字
    m_fd=socket(AF_INET,SOCK_STREAM,0);

}
NetSocket::~NetSocket(){
    close(m_fd);
}
void NetSocket::setEpollInCb(EpollInCbType func_ptr){

}
void NetSocket::setEpollOutCb(EpollOutCbType func_ptr){

}
EpollInCbType NetSocket::getEpollInCb(){
    return m_epoll_in_cb;
}
EpollOutCbType NetSocket::getEpollOutCb(){
    return m_epoll_out_cb;
}
int NetSocket::getSockFd(){
    return m_fd;
}