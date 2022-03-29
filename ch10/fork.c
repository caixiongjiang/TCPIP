#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char *argv[])
{
    pid_t pid;
    int lval = 20;
    gval++, lval += 5;

    pid = fork();//创建子进程
    if(pid == 0) {//父进程程序
        gval += 2;
        lval += 2;
    }
    else { //子进程程序
        gval -= 2;
        lval -= 2;
    }

    if(pid == 0)
        printf("Child Proc:[%d, %d] \n", gval, lval);
    else
        printf("Parent Proc:[%d, %d] \n", gval, lval);
    return 0;
}