#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sd, clnt_sd;
    FILE * fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    //fopen()函数可以用来打开文件，写法如下，第一个参数是文件名路径，第二个参数的是打开文件的方式：
    //“r”代表只读， “rb”代表只读二进制 ”t“代表 text 文本文件 
    //”b“代表 binary 二进制文件 ”+“代表both read and write 读写 ”a“代表 append 末尾追写
    fp = fopen("file_server.c", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    listen(serv_sd, 5);

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    while(1)
    {
        //Syntax: size_t fread(void *ptr, size_t size, size_t n, FILE *fp);
        //fread读取文件时，会自动保留当前的读取位置，也就是说，不用自己去管读取的位置在哪个字节，读取了对应文件的部分后，会自动继续往下读取
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE)
        {
            write(clnt_sd, buf, read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
    }

    shutdown(clnt_sd, SHUT_WR);//半关闭，断开输出流
    read(clnt_sd, buf, BUF_SIZE);//半关闭状态下读取最后正在传输的字符
    printf("Message from client: %s\n", buf);

    fclose(fp);
    close(serv_sd);
    close(clnt_sd);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}