#pragma once
#include"../base/config_read.h"
#include"../base/mysqlconn.h"
#include"../base/mongoose.h"
/*客户端向服务端请求视频列表时的api，处理回发对应数据，在此规定请求行的格式是GET /api/filesList?files=10&strategy=random;指定要返回
视频的个数和以一种怎样的方式来查找数据,比如此时就是返回10个视频并且是随机返回策略*/

void apiFilesList(void* args);
//以随机策略来构造返回的音视频文件

void constructFileListsInfoRandom(int file_cnt,char* wbuf,int wbuf_sz,char* offset);
void constructFileListsInfoUserowned(int file_cnt,char* wbuf,int wbuf_sz,char* username,char* offset);
void constructFileListsInfoUserhistory(int file_cnt,char* wbuf,int wbuf_sz,char* username,char* offset);
void constructFileListsInfoSearch(int file_cnt,char* wbuf,int wbuf_sz,char* search_key);
void constructFileListsAudio(int file_cnt,char* wbuf,int wbuf_sz);
void constructFileListsVideo(int file_cnt,char* wbuf,int wbuf_sz);
void constructFileListsAudio(int file_cnt,char* wbuf,int wbuf_sz,char* key_search);
void constructFileListsVideo(int file_cnt,char* wbuf,int wbuf_sz,char* key_search);

void fileListResponseSuccess(MYSQL_RES* res,char* wbuf,int wbuf_sz);
void fileListResponseFailed(MYSQL_RES* res,char* wbuf,int wbuf_sz);