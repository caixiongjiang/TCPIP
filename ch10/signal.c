#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//信号处理函数
void timeout(int sig)
{
    if(sig == SIGALRM)
	    puts("Time out!")
    alarm(2);
}

//信号处理函数
void keycontrol(int sig)
{
    if(sig == SIGINT)
        puts("CTRL+C pressed");
}

int main(int argc, char *argv[])
{
    int i;
    //信号处理器
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);//2s后产生SIGALRM信号
    
    //注意：一旦产生信号，调用了信号处理器，会唤醒sleep函数而离开阻塞的状态
    for(int i = 0; i < 3; ++i)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}