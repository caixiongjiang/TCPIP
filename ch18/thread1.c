#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void* thread_main(void *arg);

int main(int argc, char *argv[])
{
    pthread_t t_id;
    int thread_param = 5;

    /*
    请求创建一个线程，从thread_main函数调用开始，在单独的执行流中运行。
    同时在调用thread_main函数时向其传递thread_param变量的地址值。
    */
    //pthread_create函数：参数1--线程id 参数2--线程属性，如果为NULL则为创建默认属性的线程
    //参数3--相当于线程main函数，在单独执行流中执行的函数地址值（函数指针）
    //参数4--参数3函数的参数信息变量地址值
    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    };
    sleep(10);//停顿10s，延迟进程的终止时间。保证线程能够正常运行
    puts("end of main!");
    return 0;
}

//传入函数的参数是pthread_create函数的第四个参数
void *thread_main(void *arg)
{
    int i;
    int cnt = *((int*)arg);
    for(i = 0; i < cnt; ++i)
    {
        sleep(1);
        puts("running thread");
    }
    return NULL;
}

/*
编译(线程相关代码在编译需要添加-lpthread来声明来连接线程库，这样才能调用pthread.h中声明的函数)：
gcc thread1.c -o tr1 -lpthread
./tr1
*/