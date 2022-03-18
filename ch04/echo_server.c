#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[])
{
    int serv_sock,clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;//服务端套接字和传输套接字
    socklen_t clnt_adr_sz;//传输套接字的长度

    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    } 
    /*
    创建服务端套接字 
    参数：ipv4协议族
         面向连接
         使用的协议信息（因为只有一个就是tcp协议）所以默认为0
    */
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); 
    if(serv_sock == -1)//创建失败
        error_handling("socket() error");
    /*初始化ip地址和端口号*/
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);//自动分配服务器端地址
    serv_adr.sin_port = htons(atoi(argv[1]));
    /*
    分配地址和端口号：bind()
    参数：服务端套接字
         套接字结构体
         地址长度  
    */
    if(bind(serv_sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    /*
    进入等待连接请求状态
    参数：服务端套接字
         请求连接队列的长度
    */
    if(listen(serv_sock, 5) == 1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);

    //处理5个客户端
    for(int i = 0; i < 5; ++i)
    {
        /*
        受理客户端的连接请求
        参数：服务端套接字
             传输套接字结构体
             传输套接字长度
        成功时返回传输套接字描述符
        */
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error");
        else
            prinf("Connect client %d \n", i + 1);
        /*回声服务的实现*/
        /*读取数据
        参数：传输套接字
             接收数据的地址（这里为一个char型数组）
             接收的最大字节数
        成功时返回接收的字节数，失败时返回-1
        */
        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
            /**
             * @brief 写入数据
             * 参数：传输套接字
             *      消息的地址（char数组）
             *      写入的字节数
             */
            write(clnt_sock, message, str_len);

        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char* message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}