#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    unsigned short host_port = 0x1234;    //主机端口号，2个字节（0～65536）
    unsigned short net_port;              //网络端口号，2个字节（0～65536）
    unsigned long host_addr = 0x12345678; //主机地址号，4个字节（0～4294967296）
    unsigned long net_addr;               //网络地址号，4个字节（0～4294967296）

    net_port = htons(host_port);          //主机端口号转网络端口号（将端口号统一为大端序的过程）
    net_addr = htonl(host_addr);          //主机地址号转网络地址号（将地址号统一为大端序的过程）

    //分别打印
    printf("Host ordered port: %#x\n", host_port);
    printf("Network ordered port: %#x\n", net_port);
    printf("Host ordered addr:%#lx\n", host_addr);
    printf("Net ordered addr:%#lx\n", net_addr);
    return 0;
}