#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int send_sock;
	struct sockaddr_in mul_adr;
	int time_live = TTL;
	FILE *fp;
	char buf[BUF_SIZE];
	if(argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}	
	
	send_sock = socket(PF_INET, SOCK_DGRAM, 0);//多播数据由UDP完成的
	//设置传输数据的目标地址信息。重要的是必须把目标IP地址设置为多播地址
	memset(&mul_adr, 0, sizeof(mul_adr));
	mul_adr.sin_family = AF_INET;
	mul_adr.sin_addr.s_addr = inet_addr(argv[1]);//多播地址
	mul_adr.sin_port = htons(atoi(argv[2]));//多播端口

	//制定套接字TTL信息
	setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
	//只读打开
	if((fp = fopen("news.txt", "r")) == NULL)
		error_handling("fopen() error");
	
	while(!feof(fp)) /*广播*/
	{
		fgets(buf, BUF_SIZE, fp);
		//puts(buf);
		//复习一下sendto函数：参数1--udp套接字描述符 参数2--传输数据的缓冲地址值 参数3--待传输的数据长度
		//参数4--可选项参数，若没有则选0 参数5--目标地址信息的结构体  参数6--目标地址结构体变量的长度
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
		sleep(2);		
	}
	fclose(fp);
	close(send_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}








