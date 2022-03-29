#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t pid = fork();

    if(pid == 0) //子进程
    {
        sleep(15);
        return 24;
    }
    else //父进程
    {
        //第一个参数为目标子进程的id，如果传递的为-1，则可等待任意子进程终止
        while(!waitpid(-1, &status, WNOHANG)) 
        {
            sleep(3);
            puts("sleep 3sec");
        }

        if(WIFEXITED(status))
            printf("子进程返回值为：%d \n", WEXITSTATUS(status));
    }
    return 0;
}