#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100
void * thread_inc(void * arg);
void * thread_des(void * arg);
long long num = 0;
pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
    pthread_t thread_id[NUM_THREAD];
    int i;

    pthread_mutex_init(&mutex, NULL);//互斥量的创建

    printf("sizeof long long: %d \n", sizeof(long long));//查看long long的大小
    for(i = 0; i < NUM_THREAD; ++i)
    {
        if(i % 2)
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        else
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
    }

    for(i = 0; i < NUM_THREAD; ++i)
        pthread_join(thread_id[i], NULL);

    printf("result: %lld \n", num);
    pthread_mutex_destroy(&mutex);
    return 0;
}

//此处的互斥量设在for循环的外面，只需调用一个 lock 和 unlock函数时间上会有优势
//但在此循环结束之前，其他线程需要等待，因此需要等待很长时间
void * thread_inc(void * arg)
{
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 50000000; ++i)
        num += 1;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

//此处的互斥量设在for循环语句的里面，意味着需要调用50000000次 lock 和 unlock函数，时间会变得很慢
//但是它会让其他线程的等待时间变短
void * thread_des(void * arg)
{
    int i;
    for(i = 0; i < 50000000; ++i)
    {
        pthread_mutex_lock(&mutex);
        num -= 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}