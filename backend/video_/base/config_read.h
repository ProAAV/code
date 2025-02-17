#pragma once
#include<string>
#include<unordered_map>
class ConfRead{
public:
    ConfRead(const std::string& conf_file_name);
    ~ConfRead();
    void confLoadFile();
    const std::unordered_map<std::string,std::string>& confGetMap();
private:
    void confParseLine(std::string& line);
    std::string m_conf_file_name;
    std::unordered_map<std::string,std::string> m_conf_map;
};