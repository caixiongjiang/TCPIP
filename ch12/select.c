#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);//初始化fd_set的值，全部为0
    FD_SET(0, &reads);//将fd0的位置的值设为1（代表监视标准输入的变化，文件描述符0代表标准输入）

    /**
     * @brief 注意：不能在这里设定超时的范围 
     * timeout.tv_sec = 5;    
     * timeout.tv_usec = 5000;
     * 
     * 原因是每次调用select函数timeval成员的两个值都会背替换成“超时前剩余时间”
     * 因此需要在每次调用select函数之前重新设置超时的时间范围
     */

    while(1)
    {
        //这里将reads的值拷贝给temps，是因为调用select函数之后。除了发生变化的文件描述符
        //对应位外，剩下的所有位将初始化为0.因此为了记住初始值，必须经过这种复制过程
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        //select函数
        //返回值：0代表超时，-1代表失败，大于0的数代表发生事件的描述符的个数
        //参数：1--监视对象文件描述符数量 2--关注“是否存在待读取数据”文件描述符的fd_set
        //     3--关注“是否可传输无阻塞数据”文件描述符的fd_set 4--关注“是否发生异常”文件描述符的fd_set
        //     5--超时结构体，如果不想设置则为NULL
        result = select(1, &temps, 0, 0, &timeout);
        if(result == -1)
        {
            puts("select() error");
        }
        else if(result == 0)
        {
            puts("Time-out!");
        }
        else
        {
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
    return 0;
}