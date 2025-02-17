#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include"net_event_loop.h"
#include<sys/epoll.h>
#include<unistd.h>
#include<string.h>
#include<sys/ioctl.h>
#include"common.h"
void acceptCallback(int fd,EventLoop& evloop){
    std::cout<<"accept\n";
    struct sockaddr_in client_addr;
    socklen_t len=sizeof(struct sockaddr_in);
    int cfd=accept(fd,(struct sockaddr*)&client_addr,&len);
    //在tcp内核层面上设置tcp套接字的接收缓冲区大小为2048，实际最大接收字节为一半也就是1024
    unsigned recv_buffer_size=2048;
    setsockopt(cfd,SOL_SOCKET,SO_RCVBUF,&recv_buffer_size,sizeof(recv_buffer_size));
    if(cfd==-1){
        std::cout<<"accept error"<<'\n';
        return;
    }
    NetSocket* client_socket=new NetSocket(cfd);
    evloop.m_map[cfd]=client_socket;
    client_socket->setEpollInCb(recvCallback);
    client_socket->setEpollOutCb(sendCallback);
    evloop.epollAddFd(cfd,EPOLLIN);
}

void recvCallback(int fd,EventLoop& evloop){
    char* recv_buffer=evloop.m_map[fd]->getReadBuffer();
    int current_pos=strlen(recv_buffer);
    std::cout<<"current_pos:"<<current_pos<<"\n";
    if(RBUF_SIZE<current_pos){
        std::cout<<"buf is too small\n";
        return;
    }
    int ret=recv(fd,recv_buffer+current_pos,RBUF_SIZE-current_pos,0);
    if(ret==0){
        close(fd);
        epoll_ctl(evloop.m_epollfd,EPOLL_CTL_DEL,fd,nullptr);
        evloop.m_map.erase(fd);
        return;
    }
    int bytes_available;
    ioctl(fd, FIONREAD, &bytes_available);
    std::cout<<"bytes:"<<bytes_available<<'\n';
    std::cout<<"recv:"<<recv_buffer<<'\n';
    std::cout<<"len:"<<strlen(recv_buffer);
    if(bytes_available<=0){
        //recv完毕以后该将fd的事件置为EPOLLOUT
        evloop.epollEventMod(fd,EPOLLOUT);
    }
}

void sendCallback(int fd,EventLoop& evloop){
    char* write_buffer=evloop.m_map[fd]->getWriteBuffer();
    memcpy(write_buffer,"hhh",sizeof("hhh"));
    int ret=send(fd,write_buffer,strlen(write_buffer),0);
    if(ret==-1){
        std::cout<<"send failed\n";
        return;
    }
    else {
        std::cout<<"send num:"<<ret<<"\n";
    }
    //send完毕后设置为EPOLLIN
    evloop.epollEventMod(fd,EPOLLIN);
}


EventLoop::EventLoop(ConfRead& conf_reader):m_conf_reader(conf_reader){
    
}
EventLoop::~EventLoop(){

}
void EventLoop::serverSocketsFdCreate(){
    //打算是采用负载均衡的方式来实现服务端的套接字分配
    isNumber(m_conf_reader.confGetMap().at("server_nb_listen_port"));
        
    int nb_listen_port=std::stoi(m_conf_reader.confGetMap().at("server_nb_listen_port"));
    std::vector<int> vec_ports=separate_ports(m_conf_reader.confGetMap().at("server_listen_ports"),nb_listen_port);

    for(unsigned i=0;i<nb_listen_port;i++){
        int sfd=socket(AF_INET,SOCK_STREAM,0);
        int opt=1;
        setsockopt(sfd,SOCK_STREAM,SO_REUSEADDR,&opt,sizeof(opt));
        NetSocket* server_socket=new NetSocket(sfd);
        m_map[sfd]=server_socket;
        //绑定server_socket的m_epoll_in_cb和m_epoll_out_cb行为
        server_socket->setEpollInCb(acceptCallback);
        server_socket->setEpollOutCb(sendCallback);
        bindAddress(sfd,vec_ports[i]);
        listenEvent(sfd);
        epollAddFd(sfd,EPOLLIN);
    }
}
void EventLoop::bindAddress(int sfd,int port){
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    const char* ser_addr=m_conf_reader.confGetMap().at("server_address").c_str();
    server_addr.sin_addr.s_addr=inet_addr(ser_addr);
    bind(sfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr_in));
    std::cout<<"bind:"<<ser_addr<<" "<<port<<'\n';
}
void EventLoop::listenEvent(int sfd){
    listen(sfd,10);
}
void EventLoop::enterLoop(EventLoop& evloop){
    while(1){
        int nb_ready=epoll_wait(m_epollfd,m_epoll_events,EPOLL_EVENTS_MAX,-1);
        if(nb_ready<0){
            if(errno==EBADF){
                std::cout<<"wuxiao\n";
                return;
            }
            else if(errno==EINTR){
                std::cout<<"epoll interrput\n";
                continue;
            }
            else if(errno==EINVAL){
                std::cout<<"epoll EPOLL_EVENTS_MAX is invalid\n";
                return;
            }
            else if(errno==EFAULT){
                std::cout<<"epoll events point to area do not able to write\n";
                return;
            }
            else{
                std::cout<<"未知错误，待查询\n";
                return;
            }
        }
        
        if(nb_ready>0){
            for(unsigned i=0;i<nb_ready;i++){
                if(m_epoll_events[i].events&EPOLLIN){
                    m_map[m_epoll_events[i].data.fd]->getEpollInCb()(m_epoll_events[i].data.fd,evloop);
                }
                else if(m_epoll_events[i].events&EPOLLOUT){
                    m_map[m_epoll_events[i].data.fd]->getEpollOutCb()(m_epoll_events[i].data.fd,evloop);
                }
                else if(m_epoll_events[i].events&EPOLLERR){
                    std::cout<<"epoll err"<<'\n';
                    delete m_map[m_epoll_events[i].data.fd];
                }
            }
        }
        
    }

}   

void EventLoop::eventLoopStart(EventLoop& evloop){
    epollCreate();
    serverSocketsFdCreate();
    enterLoop(evloop);
}
void EventLoop::epollCreate(){
    m_epollfd=epoll_create(1);
}
void EventLoop::epollAddFd(int fd,int ev){
    struct epoll_event* event=regisEvent(fd,ev);
    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,fd,event);
}

struct epoll_event* EventLoop::regisEvent(int fd,int ev){
    struct epoll_event* event=new struct epoll_event;
    event->data.fd=fd;
    event->events=ev;
    return event;
}
void EventLoop::epollEventMod(int fd,int ev){
    struct epoll_event* event=regisEvent(fd,ev);
    epoll_ctl(m_epollfd,EPOLL_CTL_MOD,fd,event);
}
std::vector<int> EventLoop::separate_ports(const std::string& ports,int nb_port){
    int start=0,pos=0;
    std::vector<int> vec;
    while(1){
        pos=ports.find(' ',start);
        std::string port=ports.substr(start,pos-start+1);
        start=pos+1;
        std::cout<<"port:"<<port<<'\n';
        blankTrim(port);
        isNumber(port);
        std::cout<<"size:"<<port.size()<<"\n";
        vec.push_back(std::stoi(port));
        if(pos==std::string::npos){
            break;
        }
    }
    return vec;
}