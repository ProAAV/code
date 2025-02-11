#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include"net_event_loop.h"
#include<sys/epoll.h>
void acceptCallback(int fd){

}

void recvCallback(int fd){

}

void sendCallback(int fd){

}


EventLoop::EventLoop(){
    
}
EventLoop::~EventLoop(){

}
void EventLoop::serverSocketFdCreate(){
    m_server_socket=new NetSocket;
    m_map[m_server_socket->getSockFd()]=m_server_socket;
    //绑定server_socket的m_epoll_in_cb和m_epoll_out_cb行为
    m_server_socket->setEpollInCb(acceptCallback);
    m_server_socket->setEpollOutCb(sendCallback);
}
void EventLoop::bindAddress(){
    int sfd=m_server_socket->getSockFd();
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(9000);
    server_addr.sin_addr.s_addr=inet_addr("192.168.208.128");
    bind(sfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr_in));
    

}
void EventLoop::listenEvent(){
    listen(m_server_socket->getSockFd(),10);
}
void EventLoop::enterLoop(){
    while(1){
        int nb_ready=epoll_wait(m_epollfd,m_epoll_events,EPOLL_EVENTS_MAX,-1);
        for(unsigned i=0;i<nb_ready;i++){
            if(m_epoll_events[i].events&EPOLLIN){
                m_map[m_epoll_events[i].data.fd]->getEpollInCb();
            }
            else if(m_epoll_events[i].events&EPOLLOUT){
                m_map[m_epoll_events[i].data.fd]->getEpollOutCb();
            }
            else if(m_epoll_events[i].events&EPOLLERR){
                std::cout<<"epoll err"<<'\n';
                delete m_map[m_epoll_events[i].data.fd];
            }
        }
    }

}   

void EventLoop::eventLoopStart(){
    serverSocketFdCreate();
    bindAddress();
    listenEvent();
    epollCreate();
    epollAddFd(m_server_socket->getSockFd());
    enterLoop();
}
void EventLoop::epollCreate(){
    m_epollfd=epoll_create1(1);
}
void EventLoop::epollAddFd(int fd){
    struct epoll_event* event=regisEvent(fd,EPOLLIN);
    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,fd,event);
}
struct epoll_event* EventLoop::regisEvent(int fd,int ev){
    struct epoll_event* event=new struct epoll_event;
    event->data.fd=fd;
    event->events=ev;
    return event;
}