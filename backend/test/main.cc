#include<fstream>
#include<iostream>
#include<string>
#include<string.h>
#include<vector>
int main(){
    std::vector<std::string> file_content;
    std::ifstream m3u8_file("hhh.txt");
    std::string line="";
    while(std::getline(m3u8_file,line)){
        if(line.at(0)=='#'){
            file_content.push_back(line+'\n');
            continue;
        }
        
        line=line+"ooooooo";
        file_content.push_back(line+'\n');
    }
    m3u8_file.close();
    std::ofstream m3u8_file1("hhh.txt");

    //写回文件
    /*file_content.push_back("qqqq\n");
    file_content.push_back("wwww\n");
    file_content.push_back("eeee\n");
    file_content.push_back("rrrr\n");*/

    for(auto& i:file_content){
        std::cout<<"i:"<<i<<'\n';
        m3u8_file1<<i;
    }
    m3u8_file.close();
    return 0;
}