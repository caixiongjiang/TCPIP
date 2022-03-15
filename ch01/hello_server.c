#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>//调用socket函数需要的头文件

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    /* 创建服务器端套接字 */
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);//调用 socket 函数创建套接字
    if (serv_sock == -1)
        error_handling("socket() error");

    /* 地址信息初始化 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;//ipv4协议
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//利用常数INADDR_ANY自动获取服务器端的ip地址，并转换为网络字节序
    serv_addr.sin_port = htons(atoi(argv[1]));//初始化端口号，并将其转化网络字节序

    /* 分配地址信息 */
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)//调用 bind 函数分配ip地址和端口号
        error_handling("bind() error");
    
    if (listen(serv_sock, 5) == -1)//调用 listen 函数将套接字转为可接受连接状态
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    //调用 accept 函数受理连接请求。如果在没有连接请求的情况下调用该函数，则不会返回，直到有连接请求为止
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");
    //稍后要将介绍的 write 函数用于传输数据，若程序经过 accept 这一行执行到本行，则说明已经有了连接请求
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}