#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void * arg);
void * accu(void * arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char *argv[])
{
    pthread_t id_t1, id_t2;
    /*信号量创建*/
    //sem_init()函数：参数1--信号量地址值 参数2--传递其他值可创建多个进程共享的信号量；传递0时创建只允许1个进程内部使用的信号量
    //参数3--指定新创建的信号量初始值
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);
    
    /*线程创建*/
    //pthread_create()方法：参数1--线程id  参数2--线程类型，NULL表示默认类型
    //参数3--指定的线程函数 参数4--线程函数传递的参数
    pthread_create(&id_t1, NULL, read, NULL);
    pthread_create(&id_t2, NULL, accu, NULL);

    /*进程阻塞，进入线程*/
    //pthread_join()函数：参数1--线程id 参数2--线程函数的返回值
    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);
    
    /*信号量销毁*/
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}

void * read(void * arg)
{
    int i;
    for(i = 0; i < 5; ++i)
    {
        fputs("Input num: ", stdout);

        sem_wait(&sem_two);//信号量sem_two初值为1，-1，变成0
        scanf("%d", &num);
        sem_post(&sem_one);//信号量sem_one初值为0，+1，变成1
    }
    return NULL;
}

void * accu(void * arg)
{
    int sum = 0, i;
    for(i = 0; i < 5; ++i)
    {
        sem_wait(&sem_one);//信号量sem_one值-1，变成0
        sum += num;
        sem_post(&sem_two);//信号量sem_two值+1，变成1
    }
    printf("Result: %d \n", sum);
    return NULL;
}

/**
 * 因为线程是同步进行的，随时可能切换，所以要设置两个二进制信号量
 * 利用sem_two调用wait函数和post函数。这是为了防止在调用accu函数的线程还未取走数据的情况下，调用read函数的线程覆盖原值
 * 利用sem_one调用wait函数和post函数。这是为了防止调用read函数的线程写入新值前，accu函数取走（旧值）数据
 */