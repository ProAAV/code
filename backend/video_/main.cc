#include<iostream>
#include"base/common.h"
#include"base/net_event_loop.h"
int main(){
    EventLoop eloop;
    eloop.eventLoopStart(eloop);
    return 0;
}