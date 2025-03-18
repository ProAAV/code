#include<stdio.h>

int main(){
    char a[16]={0};
    for(int i=0;i<5;i++){
        sprintf(a,"1234567891011");
        printf("%s\n",a);
    }
    

    return 0;
}