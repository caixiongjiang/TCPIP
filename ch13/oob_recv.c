//接收端
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcnt1.h>

#define BUF_SIZE 30
void error_handling(char *message);
void urg_handler(int signo);

//把套接字设置成全局变量，是为了在信号处理函数中使用
int acpt_sock;
int recv_sock;

int main(int argc, char *argv[])
{
	struct sockaddr_in recv_adr, serv_adr;
	int str_len, state;
	socklen_t serv_adr_sz;
	struct sigaction act;
	char buf[BUF_SIZE];
	if(argc != 2) {
		printf("Usage :%s <port>\n", argv[0]);
		exit(1);
	}
	
	act.sa_handler = urg_handler;//SRGURG信号对应的信号处理函数为urg_handler
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recv_adr, 0, sizeof(recv_adr));
	recv_adr.sin_family = AF_INET;
	recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(acpt_sock, (struct sockaddr*) &recv_adr, sizeof(recv_adr)) == -1)
		error_handling("bind() error");

	if(listen(acpt_sock, 5) == -1)
		error_handling("listen() error");
	
	serv_adr_sz = sizeof(serv_adr);
	recv_sock = accept(acpt_sock, (struct sockaddr*) &serv_adr, &serv_adr_sz);

	//文件描述符recv_sock指向的套接字拥有者(F_SETOWN)改成把getpid函数返回值用作ID的进程
	//这里原本的拥有者为操作系统负责套接字所有事务的主体
	fcnt1(recv_sock, F_SETOWN, getpid());
	//收到MSG_OOB的紧急消息时，操作系统将发出SIGURG信号
	state = sigaction(SIGURG, &act, 0);

	while((str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0)) != 0)
	{
		if(str_len == -1)//接受消息失败
			continue;
		buf[BUF_SIZE] = 0;
		puts(buf);
	}
	close(recv_sock);
	close(acpt_sock);
	return 0;
}

void urg_handler(int signo)
{
	int str_len;
	char buf[BUF_SIZE];
	str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
	buf[str_len] = 0;
	printf("Urgent message: %s \n", buf);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}