//利用fdopen函数将文件描述符转化为FILE结构体指针
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int fd = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);//使用open函数穿件文件并返回文件描述符
    if(fd == -1)
    {
        fputs("file open error", stdout);
        return -1;
    }

    fp = fdopen(fd, "w");//将文件描述符转化为FILE指针
    fputs("Network c programming \n", fp);
    fclose(fp);//转化后只需将FILE指针关闭，文件描述符将成为毫无意义的整数
    return 0;
}