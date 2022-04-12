#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50 //代表epoll实例的大小，虽然只用来做操作系统的参考
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];
    int index = 1;

    struct epoll_event *ep_events;//定义epoll_event结构体
    struct epoll_event event;//定义epoll_event结构体
    int epfd, event_cnt;

    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
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

    epfd = epoll_create(EPOLL_SIZE);//创建epoll例程
    //给epoll_wait()函数的第二个参数：保存发生事件的文件描述符集合的结构体地址 动态分配内存
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    event.events = EPOLLIN;//指定监视对象的事件类型（epoll_ctl的第四个参数）
    event.data.fd = serv_sock;//将套接字描述符保存到event结构体中
    //向epfd例程注册服务端套接字，监视对象的事件类型为“需要读取数据的情况”
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while(1)
    {
        //epoll_wait()函数
        //参数1--epoll例程文件描述符 参数2--发生事件的文件描述符集合的结构体地址
        //参数3--结构体地址中可以保存的最大事件数 参数4--单位等待时间，传递-1时代表一直等待直到发生事件
        //成功则返回发生事件的文件描述符数目，失败则返回-1
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }

        printf("循环进行了%d次\n", index);
        ++index;

        for(i = 0; i < event_cnt; ++i)
        {
            if(ep_events[i].data.fd == serv_sock)
            {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d \n", clnt_sock);
            }
            else//clnt_sock
            {
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if(str_len == 0)
                {
                    //删除并关闭epoll例程
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("closed client:%d \n", ep_events[i].data.fd);
                }
                else
                {
                    //发送消息
                    write(ep_events[i].data.fd, buf, str_len);
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}