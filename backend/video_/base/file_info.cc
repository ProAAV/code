#include"file_info.h"



CfileInfo::CfileInfo(){

}
CfileInfo::~CfileInfo(){

}
std::unordered_map<std::string,std::string>& CfileInfo::getFileInfoMap(){
    return m_file_info_map;
}