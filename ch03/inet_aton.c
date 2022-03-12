#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
void error_handling(char* message);

int main(int argc, char* argv[])
{
    char* addr = "127.232.124.79";
    struct sockaddr_in addr_inet; //定义一个ipv4的结构体实体
    
    //第一个参数为需要转换的地址，第二个参数的值为ipv4结构体实体中的ip地址成员(这样就可以直接改变结构体中的ip地址信息，不需要后续单独改一次)
    if(!(inet_aton(addr, &addr_inet.sin_addr)))//成功时返回0，失败时返回1
        error_handling("Conversion error");
    else
        printf("Network ordered interger addr:%#lx \n", addr_inet.sin_addr.s_addr);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}