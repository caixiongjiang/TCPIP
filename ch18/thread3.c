#include <stdio.h>
#include <pthread.h>
//定义线程函数
void * thread_summation(void *arg);
int sum = 0;//全局变量放在数据区，数据区和栈区都是线程们可以共同“访问”的，属于共享区。

int main(int argc, char *argv[])
{
    pthread_t id_t1, id_t2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};
    //第四个参数是传入thread_summation函数的参数
    pthread_create(&id_t1, NULL, thread_summation, (void *)range1);
    pthread_create(&id_t2, NULL, thread_summation, (void *)range2);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);
    printf("result = %d \n", sum);
    return 0;
}

void * thread_summation(void * arg)
{
    int start = ((int*)arg)[0];
    int end = ((int*)arg)[1];

    while(start <= end)
    {
        sum += start;
        ++start;
    }
    return NULL;
}