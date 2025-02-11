#include"net_socket.h"

NetSocket::NetSocket(){
    //分配一个网络套接字
    m_fd=socket(AF_INET,SOCK_STREAM,0);

}
NetSocket::~NetSocket(){

}
void NetSocket::setEpollInCb(){

}
void NetSocket::setEpollOutCb(){

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