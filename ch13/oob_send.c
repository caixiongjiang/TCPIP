//发送端
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in recv_adr;
	if(argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recv_adr, 0, sizeof(recv_adr));
	recv_adr.sin_family = AF_INET;
	recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	recv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*) &recv_adr, sizeof(recv_adr)) == -1)
		error_handling("connect() error");
	
	/**
	 * @brief 
	通过MSG_OOB可选项传递数据不会加快数据传输速度；
	而且通过信号处理函数urg_handler读取数据时（在oob_recv.c中）也只能读取一个字节
	剩余数据只能通过未设置MSG_OOB可选项的普通输入函数读取
	MSG_OOB属于“带外数据”，但TCP不另外提供，只利用TCP的紧急模式进行传输
	 * 
	 */
	write(sock, "123", strlen("123"));
	send(sock, "4", strlen("4"), MSG_OOB);//MSG_OOB代表紧急传输
	wirte(sock, "567", strlen("567"));
	send(sock, "890", strlen("890"), MSG_OOB);
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}