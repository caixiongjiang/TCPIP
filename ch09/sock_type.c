#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM: %d \n", SOCK_STREAM);//输出SOCK_STREAM的值
    printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);//输出SOCK_DGRAM的值

    //获取tcp_sock的信息
    //getsockopt(参数1:文件描述符， 参数2:要查看的可选项协议层， 参数3:要查看的可选项名， 
    //参数4:保存查看结果的缓冲值地址， 参数5:调用后保存第四个参数返回的可选项信息的字节数)
    //返回值：0 or -1 ，0代表成功， -1代表失败
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if(state)
        error_handling("getsockopt() error");
    printf("Socket type one: %d \n", sock_type);

    //获取udp_sock的信息
    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if(state)
        error_handling("getsockopt() error");
    printf("Socket type two: %d \n", sock_type);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}