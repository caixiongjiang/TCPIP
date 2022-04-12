//编译时使用 -D_REENTRANT 将所有非线程安全函数转化为线程安全函数
//线程安全函数能同时被多个线程调用不会引发问题，而非线程安全函数不行
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
void* thread_main(void *arg);

int main(int argc, char *argv[])
{
    pthread_t t_id;
    int thread_param = 5;
    void * thr_ret;//定义线程返回的结果存储的地址
    
    /*
    注意pthread_create函数和pthread_join函数结尾都带一个分号
    */
    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    };

    //使用该函数使进程进入等待状态，参数2--保存线程函数返回结果
    if(pthread_join(t_id, &thr_ret) != 0)
    {
        puts("pthread_join() error");
        return -1;
    };

    printf("Thread return message: %s \n", (char*)thr_ret);
    free(thr_ret);
    return 0;
}

//线程函数
void* thread_main(void *arg)
{
    int i;
    int cnt = *((int*)arg);
    char *msg = (char*)malloc(sizeof(char*)*50);
    strcpy(msg, "Hello, I am thread~ \n");

    for(i = 0; i < cnt; ++i)
    {
        sleep(1);
        puts("running thread");
    }
    return (void*)msg;
}