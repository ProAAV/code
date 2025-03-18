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
int uploadHandleFileTLS(CfileInfo& file_info,ConfRead& conf_reader);
int uploadFileToFastdfs(CfileInfo& file_info,ConfRead& conf_reader,const bool& is_audio);
//checkEnd函数负责检查传入字符串的末尾是否是以'/'结尾，如果不是则加上一个'/'
void checkEnd(std::string& str);
int generateCoverPic(CfileInfo& file_info,ConfRead& conf_reader,bool& is_audio);
int removeFilesInTmp(ConfRead& conf_reader);
bool fileMd5InfoCheckout(CfileInfo& file_info);
void upload_responSuccess(char* wbuf,int wbuf_sz);
void upload_responFailed(char* wbuf,int wbuf_sz);
void upload_extract_file_features(std::string str);
std::string upload_video_extract_analysis(std::string str);
inline std::vector<std::string> vec_remove_file_path;
static std::vector<std::string> vec_file_features;
static std::vector<std::string> vec_file_analysis;
