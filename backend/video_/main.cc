#include<iostream>
#include"base/common.h"
#include"base/net_event_loop.h"
#include"base/config_read.h"
#include"base/thread_pool.h"
int main(int argc,char* argv[]){
    if(argc<2){
        std::cout<<"conf_file is not point\n";
        return 1;
    }

    ConfRead conf_reader(argv[1]);
    conf_reader.confLoadFile();
    thread_pool_t* thread_pool=threadpool_create(10);
    EventLoop eloop(conf_reader,thread_pool);
    eloop.eventLoopStart(eloop);
    return 0;
}