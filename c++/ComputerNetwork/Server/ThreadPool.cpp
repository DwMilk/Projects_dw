#include "ThreadPool.h"
#include <cstring>
#include <unistd.h>
#define DEFAULT_MINTHREADS 3
#define DEFAULT_MAXTHREADS 8
/* 管理者线程 */
void *manageThreadFunc(void *arg)
{
    pthread_exit(NULL);
}

/* 工作者线程 */
void *workerThreadFunc(void *arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());

    ThreadPool *pool = (ThreadPool *)arg;

    /* 核心目的：去队列中取任务 处理 */
    while (true)
    {
        pthread_mutex_lock(&(pool->m_mutex));

        while (pool->m_queueSize == 0)
        {
            std::cout << "waiting "<<std::endl;
            /* 线程同步，等待，然后解锁 */
            pthread_cond_wait(&(pool->m_queueNotEmpty), &(pool->m_mutex));
        }
        /* 从任务队列里面取数据 */
        Task_t newTask;
        newTask.function = pool->m_queue[pool->m_queueFront].function;
        newTask.arg = pool->m_queue[pool->m_queueFront].arg;

        /* 移动队头的位置 */
        pool->m_queueFront = (pool->m_queueFront + 1) % pool->m_queueCapacity;
        /* 任务队列的任务数减一 */
        (pool->m_queueSize)--;

        /* 解锁 */
        pthread_mutex_unlock(&(pool->m_mutex));

        pthread_mutex_lock(&(pool->m_busyMutex));
        /* 忙碌的线程数 */
        (pool->m_busyThreadNums)++;
        pthread_mutex_unlock(&(pool->m_busyMutex));

        /* 处理任务 */
        newTask.function(newTask.arg);

        pthread_mutex_lock(&(pool->m_busyMutex));
        /* 忙碌的线程数 */
        (pool->m_busyThreadNums)--;
        pthread_mutex_unlock(&(pool->m_busyMutex));
        /* 发信号 */
        pthread_cond_signal(&(pool->m_queueNotFull));
    }
}
ThreadPool::ThreadPool(int minThreads, int maxThreads, int maxQueueCapacity)
{
    if (minThreads <= 0 || maxThreads <= 0)
    {
        minThreads = DEFAULT_MINTHREADS;
        maxThreads = DEFAULT_MAXTHREADS;
    }
    this->m_minThreadsNums = minThreads;
    this->m_maxThreadsNums = maxThreads;
    this->m_queueCapacity = maxQueueCapacity;

    /* 任务队列初始化*/
    this->m_queue = new Task_t[this->m_queueCapacity];
    if (this->m_queue == NULL)
    {
        perror("new error.");
        /* 进程退出 */
        _exit(-1);
    }
    /* 清空脏数据 */
    memset(this->m_queue, 0, sizeof(Task_t) * this->m_queueCapacity);
    /* 队列的属性 */
    this->m_queueSize = 0;
    this->m_queueFront = 0;
    this->m_queueTail = 0;
    /* 线程的初始化 */
    this->m_threads = new pthread_t[this->m_maxThreadsNums];
    if (this->m_threads == NULL)
    {
        perror("new thread error:");
        _exit(-1);
    }
    /* 清空脏数据 */
    memset(this->m_threads, 0, sizeof(pthread_t) * this->m_maxThreadsNums);

    /* 启动管理者线程 */
    int ret = pthread_create(&(this->manageThread), NULL, manageThreadFunc, this);
    if (ret == -1)
    {
        perror("thread create error.");
        _exit(-1);
    }
    /* 忙碌的线程数 */
    this->m_busyThreadNums = 0;
    /* 存活的线程数 */
    this->m_aliveThreadNums = 0;
    /* 创建线程，数量为最小线程数 */
    for (int idx = 0; idx < minThreads; idx++)
    {
        int ret = pthread_create(&(this->m_threads[idx]), NULL, workerThreadFunc, this);
        if (ret == -1)
        {
            perror("thread create error:");
            _exit(-1);
        }
        /* 存活的线程数 + 1*/
        this->m_busyThreadNums++;
    }
    /* 锁的初始化 */
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_busyMutex, NULL);

    /* 条件变量初始化 */
    pthread_cond_init(&m_queueNotEmpty, NULL);
    pthread_cond_init(&m_queueNotFull, NULL);
}

void ThreadPool::addTask(void *(*function)(void *arg), void *arg)
{
    /* 将任务添加到任务队列中 */

    /* 上锁 */
    pthread_mutex_lock(&(this->m_mutex));
    while (this->m_queueSize == this->m_queueCapacity)
    {
        pthread_cond_wait(&(this->m_queueNotFull), &(this->m_mutex));
    }
    this->m_queue[this->m_queueTail].function = function;
    this->m_queue[this->m_queueTail].arg = arg;
    this->m_queueTail = (this->m_queueTail + 1) % this->m_queueCapacity;
    /* 任务数量 + 1 */
    this->m_queueSize++;

    /* 解锁 */
    pthread_mutex_unlock(&(this->m_mutex));
    /* 发信号 */
    pthread_cond_signal(&(this->m_queueNotEmpty));
}

/* 析构函数 */
ThreadPool::~ThreadPool()
{
}
