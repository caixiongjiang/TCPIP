#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;//接收的字节数
    socklen_t adr_sz;//接收消息的结构体的长度
    
    struct sockaddr_in serv_adr, from_adr;//目标结构体和发送方的结构体
    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = PF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    while(1) 
    {
        /**
         * @brief 
         int fputs(const char *s, FILE *stream);
         int puts(const char *s);
         返回值：成功返回一个非负整数，出错返回EOF
         fputs向指定的文件写入一个字符串，puts向标准输出写入一个字符串。
         * 
         */
        /*
        char *fgets(char *s, int size, FILE *stream);
        功能：从文件流读取一行，送到缓冲区
        参数s是缓冲区的首地址，size是缓冲区的长度，
        该函数从stream所指的文件中读取以'\n'结尾的一行(包括'\n'在内)存到缓冲区s中，并且在该行末尾添加一个'\0'组成完整的字符串。
        */
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        sendto(sock, message, BUF_SIZE, 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        adr_sz = sizeof(from_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
        message[str_len] = 0;
        printf("Message from server: %s", message);//printf遇到\0会结束
    }
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}