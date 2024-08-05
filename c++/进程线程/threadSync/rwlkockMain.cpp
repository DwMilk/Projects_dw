#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
pthread_rwlock_t g_rwlock;
int g_num = 100;
/* 线程函数1 */
/* 读锁 */
void *threadFunc1(void *arg)
{
    // while (1)
    // {
        /* 读锁 */
        pthread_rwlock_rdlock(&g_rwlock);

        for (int idx = 0; idx < 10; idx++)
        {
            cout << "threadFunc1 g_num:" << g_num << endl;
            sleep(1);
        }
        /* 解锁 */
        pthread_rwlock_unlock(&g_rwlock);
    // }

    /* 线程退出 */
    pthread_exit(NULL);
}
void *threadFunc2(void *arg)
{
    // while (1)
    // {
        /* 读锁 */
        pthread_rwlock_rdlock(&g_rwlock);

        for (int idx = 0; idx < 10; idx++)
        {
            cout << "threadFunc2 g_num:" << g_num << endl;
            usleep(1000000);
        }
        /* 解锁 */
        pthread_rwlock_unlock(&g_rwlock);
    // }
    /* 线程退出 */
    pthread_exit(NULL);
}

void *threadFunc3(void *arg)
{
    while (1)
    {
        /* 写锁 其他线程都不允许读或写操作 (既拿不到读锁也拿不到写锁)*/
        pthread_rwlock_wrlock(&g_rwlock);
        sleep(10);
        g_num += 666;
        // for (int idx = 0; idx < 10; idx++)
        // {
        //     cout << "threadFunc3 g_num:" << g_num << endl;
        //     usleep(1000);
        // }
        /* 解锁 */
        pthread_rwlock_unlock(&g_rwlock);
    }
    /* 线程退出 */
    pthread_exit(NULL);
}
int main()
{
    /* 初始化读写锁 */
    pthread_rwlock_init(&g_rwlock, NULL);

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

    /* 释放锁 */
    pthread_rwlock_destroy(&g_rwlock);
    return 0;
}