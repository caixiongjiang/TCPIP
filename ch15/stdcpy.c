//利用标准I/O函数复制文件（基于缓冲的复制）
#include <stdio.h>
#define BUF_SIZE 3

int main(int argc, char *argv[])
{
    FILE *fp1;//保存在fp1中的是FILE结构体指针
    FILE *fp2;//保存在fp2中的是FILE结构体指针
    char buf[BUF_SIZE];

    fp1 = fopen("news.txt", "r");
    fp2 = fopen("cpy.txt", "w");
    
    while(fgets(buf, BUF_SIZE, fp1) != NULL)
        fputs(buf, fp2);
    fclose(fp1);
    fclose(fp2);
    return 0;
}