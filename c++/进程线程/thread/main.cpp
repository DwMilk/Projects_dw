#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

/* 全局变量 */
int g_money = 200;

/* 线程资源共享 */
void *handleThread1(void *arg)
{
    int num = *(int *)arg;
    while (g_money > 0)
    {
        g_money -= 1;
        printf("thread1 g_money = %d\n", g_money);
        sleep(1);
    }
#if 0
    int number = 1;
    while (1)
    {
        printf("num = %d\n", number);
        number+=2;

        /* 休眠2s */
        sleep(2);
    }
#endif
    return NULL;
}
void *handleThread2(void *arg)
{
    int number = 0;
    while (g_money > 0)
    {
        g_money -= 1;
        printf("thread2 g_money = %d\n", g_money);
        /* 休眠2s */
        sleep(2);
    }

    return NULL;
}
int main()
{
#if 0
    /* 线程号Id */
    pthread_t threadId = pthread_self();
    cout << "threadId : " << threadId << endl;
    /* 进程号Id */
    pid_t pidId = getpid();
    cout << "pidId:" << pidId << endl;
#endif

#if 1
    /* 创建线程 */

    pthread_t threadId1;
    /* 局部变量 - 栈空间 */
    int num1 = 100;
    int ret = pthread_create(&threadId1, NULL, handleThread1, &num1);
    if (ret != 0)
    {
        perror("thread create error!");
        exit(-1);
    }

    pthread_t threadId2;
    int ret2 = pthread_create(&threadId2, NULL, handleThread2, NULL);
    if (ret2 != 0)
    {
        perror("thread create error!");
        exit(-1);
    }

    // while (1)
    // {
    //     printf("i an main process!\n");
    //     sleep(1);
    // }
#if 0
    /* 回收线程资源 */
    /* 一旦线程号被分离，pthread_join函数不能够再回收其他线程资源 */
    pthread_join(threadId1,NULL);
    pthread_join(threadId2,NULL);
#endif
    cout << "hello world!\n";
    int cnt = 100;
    while (cnt--)
    {
        sleep(1);
    }
#endif

    return 0;
}