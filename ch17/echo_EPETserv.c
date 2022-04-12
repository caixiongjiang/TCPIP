#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>
#define BUF_SIZE 4
#define EPOLL_SIZE 50
void setnonblockingmode(int fd);
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];
    int index = 1;

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
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
    
    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    setnonblockingmode(serv_sock);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while(1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }

        printf("循环发生了%d次\n", index);
        ++index;
        for(i = 0; i < event_cnt; ++i)
        {
            if(ep_events[i].data.fd == serv_sock)
            {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
                //非阻塞模式进行I/O
                setnonblockingmode(clnt_sock);
                //EPOLLET代表边缘触发模式
                event.events = EPOLLIN|EPOLLET;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client:%d \n", clnt_sock);
            }
            else
            {
                while(1)
                {
                    //这里需要循环读取，因为使用了非阻塞I/O,即使数据没有准备好也会进行读取。所以要循环调用，直到所有数据读取完为止。
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if(str_len == 0) {
                        //epfd例程删除clnt_sock
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client:%d \n", ep_events[i].data.fd);
                        break;
                    }
                    else if(str_len < 0) {
                        if(errno == EAGAIN)
                            break;
                    }
                    else {
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

//将套接字改为非阻塞方式
void setnonblockingmode(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);//F_GETFL代表获得第一个参数所指的文件描述符fd的属性
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);//F_SETFL代表更改文件描述符的属性 改为在原来的基础上加上O_NONBLOCK
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}