#include<fstream>
#include<iostream>
#include<string>

#include<vector>
using namespace std;
int main(){
    string s="流行音乐（Pop）,摇滚乐（Rock）,嘻哈（Hip-Hop/Rap）,电子音乐（Electronic）,爵士乐（Jazz）\
    str:流行音乐（Pop）,摇滚乐（Rock）,嘻哈（Hip-Hop/Rap）,电子音乐（Electronic）,爵士乐（Jazz）";
    int i=0;
    
    int pos=0;
    pos=s.find(",",pos);
    cout<<pos<<endl;
    string a=s.substr(pos+1,3);
    cout<<a<<endl;
    /*for(auto c:s){
        cout<<"i:"<<i<<" "<<"c:"<<c<<endl;
        i++;
    }*/
    /*for(int i=0;i<=19;i+=3){
        cout<<s[i]<<endl;
    }*/
    return 0;
}