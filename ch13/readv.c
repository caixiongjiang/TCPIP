#include <stdio.h>
#include <sys/uio.h>
#define BUF_SIZE 100

int main(int argc, char *argv[])
{
	struct iovec vec[2];
	char buf1[BUF_SIZE] = {0,};
	char buf2[BUF_SIZE] = {0,};
	int str_len;

	vec[0].iov_base = buf1;
	vec[0].iov_len = 5;
	vec[1].iov_base = buf2;
	vec[1].iov_len = BUF_SIZE;
	//readv:参数1--代币代表传输对象的文件描述符
	//参数2--iovec结构体的地址值，包含带发送数据的位置和大小信息
	//参数3--第二个参数传递的数组的长度
	str_len = readv(0, vec, 2);//0代表对象为标准输入
	printf("Read bytes: %d \n", str_len);
	printf("First message: %s \n", buf1);
	printf("Second message: %s \n", buf2);
	return 0;
}