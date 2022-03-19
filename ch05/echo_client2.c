#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;//写入的字节数，读取的总字节数，读取的单词字节数
    struct sockaddr_in serv_adr;

    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    /*创建tcp套接字*/
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    /*初始化*/
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);//string转大端序整型
    serv_adr.sin_port = htons(atoi(argv[2]));
    /*建立连接*/
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    else
        puts("Connected...........");

    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        //实现按q或者Q退出
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        //返回写入的字节数
        str_len = write(sock, message, strlen(message));
        
        recv_len = 0;//接收的字节数
        while(recv_len < str_len) 
        {
            //成功时返回读取的字节数
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if(recv_cnt == -1)
                error_handling("read() error");
            recv_len += recv_cnt;
        }
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}