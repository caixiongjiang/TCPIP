#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    struct sockaddr_in addr1, addr2;
    char* str_ptr;
    char str_arr[20];//用于长期保存

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    //成功时返回的地址会覆盖新的地址字符串
    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);//将str_ptr复制给str_arr（为了长期保存，复制到其他内存空间）
    printf("Dotted-Decimal notation1: %s \n", str_ptr);

    //再次调用该函数后，之前的str_ptr直接改变了，验证了该函数返回的是char指针
    inet_ntoa(addr2.sin_addr);//整数型ip地址转为string，并覆盖之前的str_ptr的值
    printf("Dotted-Decimal notation2: %s \n", str_ptr);//1.2.3.4覆盖之后成为了1.1.1.1
    printf("Dotted-Decimal notation3: %s \n", str_arr);
    return 0;
}