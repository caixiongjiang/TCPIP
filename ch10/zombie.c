#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();

    if(pid == 0)
    {
        puts("Hi, 我是子进程");
    }
    else {
        printf("子进程的id是%d\n", pid);
        sleep(30);//睡眠30s
    }

    if(pid == 0)
        puts("子进程结束！");
    else 
        puts("父进程结束！");
    return 0;
}