#pragma once
#include<cstdlib>
#include<string>
#include<iostream>
#include<regex>
#include<unistd.h>
inline void isNumber(const std::string& str) {
    // 正则表达式匹配整数、浮点数和科学计数法
    std::regex pattern("^[-+]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][-+]?[0-9]+)?$");
    if(!std::regex_match(str, pattern)){
        std::cout<<"---------------------------\n";
        std::cout<<str<<"is not a number\n";
        std::cout<<"---------------------------\n";

        exit(1);
    }
}
inline void blankTrim(std::string& str){
    if(str.empty()){
        return ;
    }
    int start=str.find_first_not_of(' ',0);
    int end=str.find_last_not_of(' ',str.length());
    str=str.substr(start,end-start+1);
    
    
}