#include <iostream>
using namespace std;
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/* 锁 */
pthread_mutex_t g_mutex;
/* 条件变量 */
pthread_cond_t g_cond;

struct Node
{
    /* 数据域 */
    int data;
    /* 指针域 */
    Node *next;
};

/* 链表头节点 */
Node *g_head = NULL;

/* 生产者函数 */
void *handleProduce(void *arg)
{
    while (true)
    {
        sleep(3);
        int randomNum = rand() % 100 + 1;

        /* 创建新结点 */
        Node *newNode = static_cast<Node *>(malloc(sizeof(Node) * 1));
        newNode->data = randomNum;

        /* 对共享区域上锁 */
        pthread_mutex_lock(&g_mutex);
        /* 头插 */
        newNode->next = g_head;
        g_head = newNode;

        printf("======= produce : %lu,%d\n", pthread_self(), newNode->data);
        /* 防止死锁，用完之后解锁 */
        pthread_mutex_unlock(&g_mutex);

        pthread_cond_signal(&g_cond);
        /* 休眠 */
        usleep(10000);
    }
    return NULL;
}
/* 消费者函数 */
void *handleConsum(void *arg)
{
    while (1)
    {
        /* 加锁 */
        pthread_mutex_lock(&g_mutex);
        /* 判断链表是否为空 */
        if (g_head == NULL)
        {
            /* 线程阻塞 */
            /* 等待条件变量被满足，接触阻塞，并且重新加上锁。 */
            pthread_cond_wait(&g_cond, &g_mutex); // 如果消费者线程先拿到程序，所以需要进行解锁，防止整个程序死锁
            /* 接收到阻塞之后，对互斥锁做加锁操作 */
        }
        Node *eatNode = g_head;
        g_head = g_head->next;
        /* 数据拿到之后需要解锁 */
        pthread_mutex_unlock(&g_mutex);
        printf("======= consumer : %lu,%d\n", pthread_self(), eatNode->data);
        free(eatNode);
    }
}
int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    pthread_t produce, consume;
    /* 初始化锁 */
    pthread_mutex_init(&g_mutex, NULL);
    /* 初始化条件变量 */
    pthread_cond_init(&g_cond, NULL);
    /* 生产者线程 */
    pthread_t produceId;
    int ret = pthread_create(&produceId, NULL, handleProduce, NULL);
    if (ret == -1)
    {
        perror("thread create error:");
        _exit(-1);
    }

    
    /* 消费者线程 */
    pthread_t consumeId;
    ret = pthread_create(&consumeId, NULL, handleConsum, NULL);
    if (ret == -1)
    {
        perror("thread create error:");
        _exit(-1);
    }


    /* 阻塞线程回收资源 */
    pthread_join(produceId,NULL);
    pthread_join(consumeId,NULL);
    /* 释放锁 */
    pthread_mutex_destroy(&g_mutex);
    /* 释放条件变量 */
    pthread_cond_destroy(&g_cond);
    return 0;
}