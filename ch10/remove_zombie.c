#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
	printf("去除进程id：%d \n", id);
	printf("子进程返回值为：%d \n", WEXITSTATUS(status));
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    //存储信号处理函数信息的结构体
    struct sigaction act;
    //初始化结构体
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    //SIGCHLD代表子进程结束就发出该信号
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0)//子进程区域
    {
	puts("你好，我是子进程");
	sleep(10);
	return 12;
    }
    else //父进程执行区域
    {
	printf("子进程id为：%d \n", pid);
	pid = fork();
	if(pid == 0) //第二个子进程
	{
	    puts("你好，我是子进程");
	    sleep(10);
	    exit(24);//退出返回24
	}
	else //父进程执行区域
	{
	    int i;
	    printf("子进程id：%d \n", pid);
	    for(int i = 0; i < 5; ++i)
	    {
		puts("wait...");
		sleep(5);
	    }
	}
    }
    return 0;
}