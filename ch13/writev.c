#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
	struct iovec vec[2];
	char buf1[] = "ABCDEFG";
	char buf2[] = "1234567";
	int str_len;

	vec[0].iov_base = buf1;
	vec[0].iov_len = 3;
	vec[1].iov_base = buf2;
	vec[1].iov_len = 4;
	
	//writev:参数1--代币代表传输对象的文件描述符
	//参数2--iovec结构体的地址值，包含带发送数据的位置和大小信息
	//参数3--第二个参数传递的数组的长度
	str_len = writev(1, vec, 2);//1代表对象为控制台
	puts("");
	printf("Write bytes: %d bytes", str_len);
	return 0;
}