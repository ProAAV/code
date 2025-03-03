#include<stdio.h>
#include<stdlib.h>
int main(){
    char a[100];
    char* b=(char*)malloc(100);
    printf("%ld\n",sizeof(a));
    printf("%ld\n",sizeof(b));

    return 0;
}