#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

/* 死锁解决方案
        1:检测出死锁
        2:所有的程序的加锁方式必须按照顺序
          所有的程序的解锁方式也必须按照顺序
*/

/* 全局变量 */
pthread_mutex_t g_mutex;
void painter(const char *str)
{
    if (str == NULL)
    {
        return;
    }
    while (*str != '\0')
    {
        cout << *str;
        fflush(stdout);
        sleep(1);
        /* 指针移动 */
        str++;
    }
    cout << endl;
    return;
}
/* 线程的回调函数 */
void *threadFunc1(void *arg)
{
    const char *str1 = "hello";
    /* 加锁 */
    pthread_mutex_lock(&g_mutex);
    painter(str1);
    /* 打印完之后解锁 */
    pthread_mutex_unlock(&g_mutex);
    /* 线程退出 */
    pthread_exit(NULL);
}
void *threadFunc2(void *arg)
{
    const char *str2 = "world";
    /* 加锁 */
    pthread_mutex_lock(&g_mutex);
    painter(str2);
    /* 解锁 */
    pthread_mutex_unlock(&g_mutex);
    /* 线程退出 */
    pthread_exit(NULL);
}
int main()
{
    /* 初始化锁 */

    pthread_mutex_init(&g_mutex, NULL);
    /* 创建线程1 */
    pthread_t threadId1;
    int ret = pthread_create(&threadId1, NULL, threadFunc1, NULL);
    if (ret != 0)
    {
        perror("thread create error:");
        _exit(-1);
    }

    /* 创建线程2 */
    pthread_t threadId2;
    ret = pthread_create(&threadId2, NULL, threadFunc2, NULL);
    if (ret != 0)
    {
        perror("thread create error:");
        _exit(-1);
    }

    /* 回收线程资源 */
    pthread_join(threadId1, NULL);
    pthread_join(threadId2, NULL);

    printf("i am main thread\n");

    /* 释放锁资源 */
    pthread_mutex_destroy(&g_mutex);
    return 0;
}



/* 
    运行中的程序卡死
        1：内核态阻塞
        2：用户态阻塞（死循环 & 进程占用的cpu过高）
*/