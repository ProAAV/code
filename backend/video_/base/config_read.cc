#include"config_read.h"
#include"common.h"
#include<string.h>
#include<fstream>
#include<iostream>
ConfRead::ConfRead(const std::string& conf_file_name):m_conf_file_name(conf_file_name){

}
ConfRead::~ConfRead(){

}
void ConfRead::confParseLine(std::string& line){
    if(line.empty()){
        return;
    }
    int pos=line.find('#',0);
    if(pos!=std::string::npos){
        //找到了'#'，说明'#'之前的内容才是信息
        std::string avail_str=line.substr(0,pos-0);
        return;
    }
    //没找到，说明整个line都是信息
    pos=line.find('=',0);
    std::string str_key=line.substr(0,pos-0);
    std::string str_value=line.substr(pos+1,line.length()-pos);
    blankTrim(str_key);
    blankTrim(str_value);
    
    m_conf_map[str_key]=str_value;
}
void ConfRead::confLoadFile(){
    std::ifstream file(m_conf_file_name);
    if(!file.is_open()){
        std::cout<<m_conf_file_name<<" is failed to open\n";
        return;
    }
    std::string line;
    while(std::getline(file,line)){
        confParseLine(line);
    }
}

const std::unordered_map<std::string,std::string>& ConfRead::confGetMap(){
    return m_conf_map;
}
