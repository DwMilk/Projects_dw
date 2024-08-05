#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

/* 线程是依托于进程而存在的 */
void * thread_func(void * arg)
{
    int cnt = 10;
    while(cnt--)
    {
        printf("hello world!\n");
        sleep(1);
    }
    /* 线程退出 */
    pthread_exit(NULL);
}
int main()
{
    /* 初始化线程参数 */
    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);


    /* 设置线程分离状态 */
    int ret = pthread_attr_setdetachstate(&threadAttr,PTHREAD_CREATE_DETACHED);
    if(ret != 0)
    {
        perror("setdetachstate error");
        _exit(-1);
    }

    pthread_t threadId;
    ret = pthread_create(&threadId,&threadAttr,thread_func,NULL);
    if(ret != 0)
    {
        perror("thread creatre error:");
        _exit(-1);
    }

    /* 释放线程属性 */
    pthread_attr_destroy(&threadAttr);

    while(1)
    {
        sleep(1);
    }
    return 0;
}