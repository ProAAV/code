#include<iostream>
#include"base/common.h"
#include"base/net_event_loop.h"
#include"base/config_read.h"
int main(int argc,char* argv[]){
    if(argc<2){
        std::cout<<"conf_file is not point\n";
        return 1;
    }
    ConfRead conf_reader(argv[1]);
    conf_reader.confLoadFile();
    EventLoop eloop;
    eloop.eventLoopStart(eloop);
    return 0;
}