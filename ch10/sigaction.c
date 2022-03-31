//sigaction在unix系列的不同操作系统都相同，但signal函数可能不同
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
	puts("Time out!");
    alarm(2);
}

int main(int argc, char *argv[])
{
    int i;
    struct sigaction act;//存储信号处理函数的信息
    
    //初始化信号处理函数的信息
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    /*参数1：信号类型 参数2：信号处理函数的信息（结构体） 参数3：获取之前注册的信号处理函数的指针，若不需要则传递0*/
    sigaction(SIGALRM, &act, 0);

    alarm(2);

    for(i = 0; i < 3; ++i)
    {
	puts("wait...");
	sleep(100);
    }
    return 0;
}