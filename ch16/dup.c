#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int cfd1, cfd2;
    char str1[] = "Hi~ \n";
    char str2[] = "It's nice day~ \n";

    cfd1 = dup(1);//调用dup函数复制了文件描述符1
    cfd2 = dup2(cfd1, 7);//调用dup2函数再次复制了文件描述符，并指定描述符整数值为7

    printf("fd1 = %d, fd2 = %d \n", cfd1, cfd2);
    //利用复制出的文件描述符进行输出
    write(cfd1, str1, sizeof(str1));
    write(cfd2, str2, sizeof(str2));

    //终止复制的文件描述符。但仍然有一个文件描述符，因此可以进行输出。
    close(cfd1);
    close(cfd2);
    write(1, str1, sizeof(str1));
    //终止最后的文件描述符
    close(1);
    write(1, str2, sizeof(str2));
    return 0;
}