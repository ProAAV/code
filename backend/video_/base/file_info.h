#pragma once
#include<unordered_map>
#include<string>


class CfileInfo{
public:
    CfileInfo();
    ~CfileInfo();
    std::unordered_map<std::string,std::string>& getFileInfoMap();
private:
    std::unordered_map<std::string,std::string> m_file_info_map;
};