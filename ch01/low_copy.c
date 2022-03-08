/*
第一章课后练习7：根据low_open.c写一个复制文件low_copy.c(使用底层文件I/O)
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
void error_handling(char* message);

int main(void){
    int fd1, fd2;
    char buf[100];

    fd1 = open("data.txt", O_RDONLY);//只读打开
    if(fd1 == -1) 
        error_handling("open() error!");
    printf("file descriptor: %d \n", fd1);

    //调用read函数向第11行中声明的数组buf保存读入的数据
    if(read(fd1, buf, sizeof(buf)) == -1)
        error_handling("read() error!");    
    
    fd2 = open("copy.txt", O_CREAT|O_WRONLY|O_TRUNC);//创建并只写打开
    if(fd2 == -1) 
        error_handling("open() error!");
    printf("file descriptor: %d \n", fd2);
    
    if(write(fd2, buf, sizeof(buf)) == -1)
        error_handling("write() error!");
    close(fd1);
    close(fd2);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}