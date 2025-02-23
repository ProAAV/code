#pragma once
#include<string>
#include<vector>
#include"../base/file_info.h"
#include"../base/config_read.h"
extern "C"{
    #include"../base/mongoose.h"
}
#define FASTDFS_CLIENT_CONF "/etc/fdfs/client.conf"
void apiUpload(char* wbuf,int wbuf_sz,struct mg_http_message hm,ConfRead& conf_reader);
void uploadHandleFileTLS(CfileInfo& file_info,ConfRead& conf_reader);
void uploadFileToFastdfs(CfileInfo& file_info,ConfRead& conf_reader);
//checkEnd函数负责检查传入字符串的末尾是否是以'/'结尾，如果不是则加上一个'/'
void checkEnd(std::string& str);
int generateCoverPic(CfileInfo& file_info,ConfRead& conf_reader);
void removeFilesInTmp(ConfRead& conf_reader);
inline std::vector<std::string> vec_remove_file_path;