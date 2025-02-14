#include"config_read.h"
#include<fstream>
#include<iostream>
ConfRead::ConfRead(const std::string& conf_file_name):m_conf_file_name(conf_file_name){

}
ConfRead::~ConfRead(){

}
void ConfRead::confParseLine(){

}
void ConfRead::confLoadFile(){
    std::ifstream file(m_conf_file_name);
    if(!file.is_open()){
        std::cout<<m_conf_file_name<<" is failed to open\n";
        return;
    }
    std::string line;
    while(std::getline(file,line)){
        std::cout<<"line:"<<line<<'\n';
    }
}