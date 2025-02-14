#pragma once
#include<string>
class ConfRead{
public:
    ConfRead(const std::string& conf_file_name);
    ~ConfRead();
    void confLoadFile();
private:
    void confParseLine();
    std::string m_conf_file_name;
};