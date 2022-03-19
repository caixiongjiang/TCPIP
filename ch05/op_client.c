#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4 //待运算数字的字节数大小
#define OPSZ 4  //运算结果的字节数
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SIZE]; //数据收发缓冲数组
    int result, opnd_cnt, i; //计算结果，计算的数的个数，用于做循环的量
    struct sockaddr_in serv_adr;
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    /*socket（）*/
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    /*初始化*/
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));
    /*connect（）*/
    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected...........");
    
    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt); //输入要计算的数的个数
    opmsg[0] = (char)opnd_cnt; //int转char

    for (i = 0; i < opnd_cnt; i++)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int *)&opmsg[i * OPSZ + 1]); //char数组转为int指针类型
    }
    //在输入字符之前，调用fgets函数删除缓冲中的字符串\n
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]); //输入运算符信息，保存到opmsg数组
    /*调用write函数一次性传输opmsg数组中的运算信息。
      可以调用一次write函数进行传输，也可以分成多次调用，因为tcp中不存在数据边界。
    */
    write(sock, opmsg, opnd_cnt * OPSZ + 2); 
    //保存服务器端传输的结果，因为待接收数据的长度为4字节，因此调用一次read函数就可以接收
    read(sock, &result, RLT_SIZE);

    printf("Operation result: %d \n", result);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}