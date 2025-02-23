#pragma once
#include"../base/config_read.h"
#include"../base/mongoose.h"
/*客户端向服务端请求视频列表时的api，处理回发对应数据，在此规定请求行的格式是GET /api/filesList?files=10&strategy=random;指定要返回
视频的个数和以一种怎样的方式来查找数据,比如此时就是返回10个视频并且是随机返回策略*/

void apiFilesList(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
//以随机策略来构造返回的音视频文件

void constructFileRandom(int file_cnt);