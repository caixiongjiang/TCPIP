#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if(argc != 2) {
	    printf("Usage %s <port>\n", argv[0]);
	    exit(1);
    }
    
    //结构体初始化
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	    error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
	    error_handling("listen() error");
    

    /*注意这里的父进程和子进程的套接字其实是同一个。(fork的时候子进程几乎将父进程的大部分信息都复制下来了，包括套接字描述符)
      因为严格来说套接字属于操作系统，socket函数只是返回一个文件描述符指向该套接字
      而且只有关闭所有指向该套接字的文件描述符，该套接字才能关闭
    */
    while(1)
    {
	    adr_sz = sizeof(clnt_adr);
	    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
	    if(clnt_sock == -1)
	        continue;
	    else
	        puts("新的客户端连接了！");
	
	    pid = fork();
	    if(pid == -1)//创建子进程失败，就重新创建
	    {
	        close(clnt_sock);
	        continue;
	    }
	    if(pid == 0) //子进程执行区域
	    {
	        close(serv_sock);//关闭子进程的服务套接字
	        while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)//读取长度不为0
	    	    write(clnt_sock, buf, str_len);

	        close(clnt_sock);//关闭子进程的处理客户端连接请求的套接字
	        puts("客户端断开连接");
	        return 0;
	    }   
	    else
	        close(clnt_sock);//关闭父进程的客户端连接处理套接字
        
    }
    close(serv_sock);
    return 0;
}

//信号处理函数（去除僵尸进程，返回去除的进程id）
void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("去除的进程id为：%d \n", pid);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
