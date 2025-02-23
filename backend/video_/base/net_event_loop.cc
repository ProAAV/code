#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include"net_event_loop.h"
#include<sys/epoll.h>
#include<unistd.h>
#include<string.h>
#include<sys/ioctl.h>
#include"http_layer.h"
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
    /*正确的做法应当是通过获取content-length的方式来判断后续内容是否还要继续接收，从而决定注册EPOLLIN还是
    EPOLLOUT*/
    char* recv_buffer=evloop.m_map[fd]->getReadBuffer();
    int ret=recv(fd,recv_buffer,RBUF_SIZE,0);
    std::cout<<"first recv1:"<<recv_buffer<<'\n';
    std::cout<<"recv size:"<<strlen(recv_buffer)<<'\n';
    if(ret==0){
        std::cout<<"fd disconnect!!!\n";
        close(fd);
        epoll_ctl(evloop.m_epollfd,EPOLL_CTL_DEL,fd,nullptr);
        evloop.m_map.erase(fd);
        return;
    }
    HttpServer hserver(recv_buffer,evloop.m_conf_reader);
    std::string method=hserver.http_parse_method();
    int content_length=hserver.http_get_content_length_from_request_header();
    if(content_length+hserver.http_get_header_size()>RBUF_SIZE){
        std::cout<<"rbuffer size is too small\n";
        //不够直接断开此次连接
        close(fd);
        epoll_ctl(evloop.m_epollfd,EPOLL_CTL_DEL,fd,nullptr);
        evloop.m_map.erase(fd);
        return;
    }
    //做的是循环接收，条件是请求方法POST并且接收到的请求体大小小于请求头中内容长度

    if(method=="POST"&&strlen(recv_buffer)-hserver.http_get_header_size()<content_length){
        while(1){
            int current_pos=strlen(recv_buffer);
            recv(fd,recv_buffer+current_pos,RBUF_SIZE-current_pos,0);
            std::cout<<"recv:"<<recv_buffer<<'\n';
            if(strlen(recv_buffer)-hserver.http_get_header_size()==content_length){
                std::cout<<"break\n";
                break;
            }
        }
    }
    else{
        std::cout<<"other method\n";
    }
    //到现在是彻底在这之前接受完数据了，接下来要做的就是对完整的http请求进行解析和路由并且回复
    HttpServer http_layer(recv_buffer,evloop.m_conf_reader);
    char* write_buffer=evloop.m_map[fd]->getWriteBuffer();
    int wbuffer_size=evloop.m_map[fd]->getWriteBufferSize();
    
    if(method=="POST"||method=="GET"){
        http_layer.http_route_url(write_buffer,wbuffer_size);
    }
    else{
        std::cout<<"no func can be called\n";
    }
    //接下来是一次性接收完毕的情况要处理的工作
    evloop.epollEventMod(fd,EPOLLOUT);

    
}

void sendCallback(int fd,EventLoop& evloop){
    std::cout<<"sendcb\n";
    char* write_buffer=evloop.m_map[fd]->getWriteBuffer();
    char* response=(char*)"HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\nHello, World!";
    memcpy(write_buffer,response,strlen(response));
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
    std::cout<<"sendcb over\n";
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
        blankTrim(port);
        isNumber(port);
        vec.push_back(std::stoi(port));
        if(pos==std::string::npos){
            break;
        }
    }
    return vec;
}