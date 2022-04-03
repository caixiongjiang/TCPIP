//单管道双向通信
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int fds[2];
    char str1[] = "我是谁？";
    char str2[] = "你是谁？";
    char buf[BUF_SIZE];//用于接收信息的缓存的地址
    pid_t pid;

    pipe(fds);
    pid = fork();
    if(pid == 0) 
    {
        write(fds[1], str1, sizeof(str1));
        sleep(2);//数据进入管道成为无主数据，不延迟，子进程将读取自己发送的数据
        read(fds[0], buf, BUF_SIZE);
        printf("子进程收到的消息：%s\n", buf);
    }
    else
    {
        read(fds[0], buf, BUF_SIZE);
        printf("父进程收到的消息：%s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);//防止子进程终止前提出命令提示符，因为父进程结束，其子进程也结束了。
    }
    return 0;
}