#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;//fd_max代表最大的文件描述符的号码
    char buf[BUF_SIZE];
    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    FD_ZERO(&reads);//初始化reads指向的变量
    FD_SET(serv_sock, &reads);//在reads指向的变量中注册serv_sock的信息
    fd_max = serv_sock;//目前最大的文件描述符为serv_sock，因为文件描述符号码顺序增加

    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        //文件描述符的数量 = 最大的文件描述符号码 + 1； 因为文件描述符是从0开始的
        if((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)//select()失败
            break;
        if(fd_num == 0)//超时
            continue;
        
        for(i = 0; i < fd_max + 1; ++i)
        {
            if(FD_ISSET(i, &cpy_reads))//查找到cpy_reads的信息
            {
                if(i == serv_sock)//正确连接
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads);//在reads指向的变量中注册clnt_sock的信息
                    //更新最大的描述符号码数字
                    if(fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                }
                else//read message
                {
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0)//关闭连接
                    {
                        FD_CLR(i, &reads);//reads指向的变量中删除clnt_sock的信息
                        close(i);//关闭clnt_sock
                        printf("closed client: %d \n", i);
                    }
                    else
                    {
                        write(i, buf, str_len);//发送消息
                    }
                }
            }
        }
    }
    close(serv_sock);//关闭serv_sock
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}