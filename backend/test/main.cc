extern "C"{
    #include<libavcodec/avcodec.h>
    #include<libavformat/avformat.h>
    #include<libavutil/avutil.h>
    #include<libswscale/swscale.h>
}
#include<iostream>

int main(){
    avformat_network_init();
    std::cout<<"dsdsdsd\n";
    return 0;
}