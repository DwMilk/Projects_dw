# 操作系统

## IO接口

### 1.文件打开关闭

```c++
#include <iostream>
#include <cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
    const char *filename = "test.txt";

    /* 打开文件 */
    int fd1 = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd1 == -1)
    {
        perror("open error: ");
    }
    else
    {
        cout << "fd1: " << fd1 << endl;
    }

    /* 打开文件 */
    int fd2 = open(filename, O_RDWR | O_CREAT);
    cout << "fd2: " << fd2 << endl;

    /* 打开文件 */
    int fd3 = open(filename, O_RDWR | O_CREAT);
    cout << "fd3: " << fd3 << endl;

    /* 关闭文件 */
    close(fd1);

    /* 打开文件 */
    int fd4 = open(filename, O_RDWR | O_CREAT);
    cout << "fd4: " << fd4 << endl;

    /* 关闭文件 */
    close(fd2);
    /* 关闭文件 */
    close(fd3);
    /* 关闭文件 */
    close(fd4);

    return 0;
}
```

### 2.文件的读写操作

```c++
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#define BUFFER_SIZE 1024

using namespace std;

void readFile(char *buffer, const char *filepath)
{
    /* 打开文件 */
    int fd1 = open(filepath, O_RDONLY);
    if (fd1 == -1)
    {
        /* 标准出错: STDERR_FILENO 标准错误的文件描述符 2 */
        perror("打开路径文件失败! ");
    }

    bzero(buffer, sizeof(buffer));

    /* 文件指针, 定位开头 */
    size_t readBytes = read(fd1, buffer, sizeof(buffer) - 1);
    if(readBytes == -1)
    {
        perror("读取源文件失败：");
    }
    else
    {
        buffer[readBytes] = '\0';
    }
    // printf("readBytes = %ld, \tbuffer: %s\n", readBytes, buffer);

    /* 关闭文件 */
    close(fd1);
}

void writeFile(char *buffer, char *filepath)
{
    /* 打开文件 */
    int fd1 = open(filepath, O_RDWR | O_APPEND | O_CREAT);
    if (fd1 == -1)
    {
        /* 标准出错: STDERR_FILENO 标准错误的文件描述符 2 */
        perror("新文件创建失败 或 无法打开! ");
    }

    // bzero(buffer, sizeof(buffer));

    /* 文件指针, 定位开头 */
    size_t readBytes = write(fd1, (const void*)buffer, sizeof(buffer) - 1);
    if(readBytes == -1)
    {
        perror("写入新文件失败: ");
    }
    // printf("readBytes = %ld, \tbuffer: %s\n", readBytes, buffer);

    /* 关闭文件 */
    close(fd1);
}

/* 命令行参数 */
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "命令行参数错误, 请重新执行!!! " << endl;
        exit(0);
    }

    char buffer[BUFFER_SIZE];

    readFile(buffer,argv[1]);
    writeFile(buffer,argv[2]);

    return 0;
}
```

## IPC管道

### 1.无名管道 pipe

```c++
#include <iostream>
using namespace std;
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>
#define BUFFER_SIZE 32

int main()
{
    /* 创建管道 */
    int pipefd[2];
    /* 清空脏数据 */
    memset(pipefd, 0, sizeof(pipefd));
    if(pipe(pipefd) == -1)
    {
        perror("pipe error");
        _exit(-1);
    }

    /* 创建子进程 */
    pid_t pid = fork();
    if (pid > 0)
    {
        /* 关闭父进程管道读端. */
        /* 关闭父进程的写端 */
        close(pipefd[0]);
        int num = 666;
        cout << "I am parent process! " << endl;

        string res = "hello world";
        /* 父进程负责写数据. */
        write(pipefd[1], res.c_str(), res.size());

        /* 父进程负责读数据 */
        // char readBuffer[BUFFER_SIZE] = {0};
        // read(pipefd[0], readBuffer, sizeof(readBuffer));
        // printf("parent readBuffer = %s\n", readBuffer);

        /* 回收子进程的资源 */
        wait(NULL);
    }
    else if (pid == 0)
    {
        /* 关闭子进程的写端 */
        close(pipefd[1]);

        /* 休眠1s */
        sleep(2);
        cout << "I am child process!" << endl;

        /* 得到文件描述符的状态标记. */
        int flags = fcntl(pipefd[0], F_GETFL);
        cout << "flags: " << flags << endl;
        /* 设置文件描述符位非阻塞 */
        fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

        /* 子进程负责度读, (read是一个阻塞函数: 没有函数是阻塞的， 只有文件描描述符是阻塞的) */
        char buffer[BUFFER_SIZE] = {0};
        read(pipefd[0], buffer, sizeof(buffer));

        printf("child: hello world\n");
        printf("buffer = %s\n", buffer);
    }
    return 0;
}
```

### 2.有名管道 fifo

```c++
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
using namespace std;
#define BUFFER_SIZE 16

int main()
{
    /* 创建命名管道 */
    const char *filename = "./fifoInfo";
    int ret = mkfifo(filename, 0644);
    if (ret != 0 && errno != EEXIST)
    {
        /*  */
        perror("mkfifo error: ");
        exit(-1);
    }

    /* 创建进程 */
    pid_t pid = fork();
    if (pid > 0)
    {
        /* 父进程向有名管道写数据 */
        printf("parent process\n");

        int fd = open(filename, O_RDWR);
        if (fd == -1)
        {
            perror("open error: ");
            _exit(-1);
        }

        int cnt = 3;
        const char *buf = "hello world";

        int writeBytes = 0;
        while (cnt--)
        {
            /* 向FIFO中写数据 */
            writeBytes = write(fd, buf, strlen(buf));
            if (writeBytes > 0)
            {
                cout << "writeBytes: " << writeBytes << endl;
            }
            else if (writeBytes == 0)
            {
                cout << "writeBytes == 0" << endl;
            }
            else if (writeBytes < 0)
            {
                perror("write error: ");
            }
            // sleep(1);
        }

        /* 回收子进程资源 */
        wait(NULL);
        /* 关闭文件描述符号 */
        close(fd);
    }
    else if (pid == 0)
    {
        sleep(6);
        /* 子进程向有名管道读数据 */
        printf("child process...\n");

        int fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            perror("open file error: ");
            _exit(-1);
        }

        /* 设置非阻塞 */
#if 1
        /* 得到文件描述符的状态标记. */
        int flags = fcntl(fd, F_GETFL);
        cout << "flags: " << flags << endl;
        /* 设置文件描述符位非阻塞 */
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif

        char buffer[BUFFER_SIZE] = {0};
        int readBytes = 0;
        while (1)
        {
            readBytes = read(fd, buffer, sizeof(buffer) - 1);
            if (readBytes > 0)
            {
                printf("readBytes = %d,\tbuffer = %s\n", readBytes, buffer);
            }
            else if (readBytes == 0)
            {
                cout << "readBytes == 0" << endl;
                break;
            }
            else
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    cout << "No data available now, trying again..." << endl;
                    sleep(1);
                    break;
                }
                close(fd);
                exit(-1);
            }
            sleep(1);
            /* 清楚脏数据 */
            memset(buffer, 0, BUFFER_SIZE);
        }
        /* 关闭文件 */
        close(fd);
    }
    else
    {
        perror("fork error: ");
        exit(-1);
    }

    /* 删除命名管道 */
    unlink(filename);

    return 0;
}
```



### 3.共享内存

```c++
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/mman.h>
using namespace std;
#define BUFFER_SIZE 128

int main()
{
    /* 文件名 */
    const char *filename = "./text.txt";

    int fd = open(filename, O_RDWR);
    if(fd == -1)
    {
        perror("open error");
        exit(-1);
    }
    /* 获取文件大小 */
    int length = lseek(fd, 0, SEEK_END);
    // cout << length << endl;

    void *addr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
    {
        perror("mmap error:");
        exit(-1);
    }
    /* 关闭文件描述符 */
    close(fd);

    /* 创建进程 */
    pid_t pid = fork();
    if(pid > 0)
    {
        cout << "parent process... " << endl;

        /* 父进程 - 负责写 */
        const char *str = "hello world. how are you. I am fine, thank you.";
        /* 字符串拷贝. */
        strncpy(static_cast<char*>(addr), str, strlen(str) + 1);
    }
    else if(pid == 0)
    {
        cout << "child process... " << endl;

        /* 子进程 - 负责读 */
        usleep(1000);

        /* 地址拷贝. */
        char buffer[BUFFER_SIZE] = { 0 };
        memcpy(buffer, addr, sizeof(buffer));

        // printf("%s\n", static_cast<char *>(addr));
        cout << "buffer: " << buffer << endl;
    }
    else
    {
        perror("fork error: ");
        exit(-1);
    }

    /* 接触映射 */
    munmap(addr, length);

    return 0;
}
```

### 4.信号量

```c++
#include <iostream>
using namespace std;

#include <pthread.h>
#include <unistd.h>
/* 信号量的头文件 */
#include <semaphore.h>

/* 信号量 */
sem_t g_sem;

void printer(const char *str)
{
    if( str == NULL)
    {
        return;
    }

    /* P操作: 信号量减 1 */
    sem_wait(&g_sem);

    while( *str != '\0')
    {
        cout << *(str++);
        fflush(stdout);
        usleep(500000);
    }
    cout << endl;

    /* V操作: 信号量加 1 */
    sem_post(&g_sem);
}

void *thread_func1(void *arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());

    const char *str = "hello";
    printer(str);

    /* 线程退出 */
    pthread_exit(NULL);
}

void *thread_func2(void *arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());

    const char *str = "world";
    printer(str);

    /* 线程退出 */
    pthread_exit(NULL);
}

int main()
{
    /* 初始化信号量 */
    int initVal = 1;
    sem_init(&g_sem, 0, initVal);

    pthread_t tid1;
    int ret = pthread_create(&tid1, NULL, thread_func1, NULL);
    if (ret != 0)
    {
        perror("thread create error!");
        _exit(-1);
    }

    pthread_t tid2;
    ret = pthread_create(&tid2, NULL, thread_func2, NULL);
    if (ret != 0)
    {
        perror("thread create error!");
        _exit(-1);
    }

    int cnt = 60;
    while (cnt--)
    {
        sleep(1);
    }

    /* 销毁信号量 */
    sem_destroy(&g_sem);

    return 0;
}
```

## 线程同步ThreadSync

### 1.互斥锁

```c++
#include <iostream>

using namespace std;
#include <unistd.h>
#include <pthread.h>

/* 全局变量锁 */
pthread_mutex_t g_mutex;

void painter(const char *str)
{
    if (str == NULL)
    {
        return;
    }

    while (*str != '\0')
    {
        cout << *(str++);
        fflush(stdout);
        usleep(300000);
    }
    cout << endl;
}

void *threadFunc1(void *arg)
{
    const char *str1 = "hellohellohellohellohellohellohello";

    /* 加锁 */
    pthread_mutex_lock(&g_mutex);
    painter(str1);
    /* 解锁 */
    pthread_mutex_unlock(&g_mutex);

    /* 退出 */
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

    pthread_exit(NULL);
}

int main()
{
    /* 初始化🔒 */
    pthread_mutex_init(&g_mutex, NULL);

    /* 创建线程1 */
    pthread_t threadID1;
    int ret = pthread_create(&threadID1, NULL, threadFunc1, NULL);
    if (ret != 0)
    {
        perror("thread1 create error");
        _exit(-1);
    }

    /* 创建线程2 */
    pthread_t threadID2;
    ret = pthread_create(&threadID2, NULL, threadFunc2, NULL);
    if (ret != 0)
    {
        perror("thread2 create error");
        _exit(-1);
    }

    /* 等待线程结束, 回收线程资源 */
    pthread_join(threadID1, NULL);
    pthread_join(threadID2, NULL);

    printf("I am process\n");

    /* 释放锁资源 */
    pthread_mutex_destroy(&g_mutex);

    return 0;
}

```

### 2.读写锁

```c++
#include <iostream>
#include <pthread.h>
#include <unistd.h>

/* 读写锁 */
pthread_rwlock_t g_rwlock;
int g_num = 100;

/* 线程函数1 */
/* 读锁 */
void *threadFunc1(void *arg)
{
    // while (1)
    // {
    sleep(1);

    /* 上读锁 */
    pthread_rwlock_rdlock(&g_rwlock);

    for (int idx = 0; idx < 10; idx++)
    {
        std::cout << "threadFunc1 g_num: " << g_num << std::endl;
        sleep(1);
    }

    /* 解读锁 */
    pthread_rwlock_unlock(&g_rwlock);
    // }
    /* 线程退出 */
    pthread_exit(NULL);
}

/* 线程函数2 */
/* 读锁 */
void *threadFunc2(void *arg)
{
    // while (1)
    // {
    sleep(1);
    /* 上读锁 */
    pthread_rwlock_rdlock(&g_rwlock);

    for (int idx = 0; idx < 10; idx++)
    {
        std::cout << "threadFunc2 g_num: " << g_num << std::endl;
        sleep(1);
    }
    /* 解读锁 */
    pthread_rwlock_unlock(&g_rwlock);
    // }
    /* 线程退出 */
    pthread_exit(NULL);
}

/* 线程函数3 */
/* 写锁 */
void *threadFunc3(void *arg)
{
    /* 写锁, 其他线程都不允许读或者写操作, (既拿不到读锁也拿不到写锁) */
    pthread_rwlock_wrlock(&g_rwlock);

    /* 休眠10s. */
    sleep(10);

    /* 解锁 */
    pthread_rwlock_unlock(&g_rwlock);
    /* 线程退出 */
    pthread_exit(NULL);
}

/* 线程函数4 */
/* 写锁 */
void *threadFunc4(void *arg)
{

    /* 线程退出 */
    pthread_exit(NULL);
}

int main()
{
    /* 初始化读写锁 */
    pthread_rwlock_init(&g_rwlock, NULL);

    /* 创建线程1 */
    pthread_t threadID1;
    int ret = pthread_create(&threadID1, NULL, threadFunc1, NULL);
    if (ret != 0)
    {
        perror("thread1 create error");
        _exit(-1);
    }

    /* 创建线程2 */
    pthread_t threadID2;
    ret = pthread_create(&threadID2, NULL, threadFunc2, NULL);
    if (ret != 0)
    {
        perror("thread2 create error");
        _exit(-1);
    }

    /* 创建线程3 */
    pthread_t threadID3;
    ret = pthread_create(&threadID3, NULL, threadFunc3, NULL);
    if (ret != 0)
    {
        perror("thread1 create error");
        _exit(-1);
    }

    /* 创建线程4 */
    pthread_t threadID4;
    ret = pthread_create(&threadID4, NULL, threadFunc4, NULL);
    if (ret != 0)
    {
        perror("thread2 create error");
        _exit(-1);
    }

    /* 等待线程结束, 回收线程资源 */
    pthread_join(threadID1, NULL);
    pthread_join(threadID2, NULL);
    pthread_join(threadID3, NULL);
    pthread_join(threadID4, NULL);

    printf("I am process\n");

    /* 释放读写锁 */
    pthread_rwlock_destroy(&g_rwlock);

    return 0;
}
```

### 3.生产者消费者模型

```c++
#include <iostream>

using namespace std;

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory>

/* 线程同步 - 互斥锁 */
pthread_mutex_t g_mutex;

/* 阻塞线程 - 条件变量 */
pthread_cond_t g_cond;

/* 结点结构 */
struct Node
{
    /* 数据域 */
    int data;
    /* 指针域 */
    std::shared_ptr<Node> next;
};

/* 链表的头结点 */
// Node *g_head = NULL;
std::shared_ptr<Node> g_head = nullptr;

/* 生产者函数 */
void *handleProduce(void *arg)
{
    while (1)
    {
        int randomNum = rand() % 100 + 1;

        /* 创建一个结点 */
        // Node *newNode = static_cast<Node *>(malloc(sizeof(Node) * 1));
        std::shared_ptr<Node> newNode = std::make_shared<Node>();
        /* 结点初始化 */
        newNode->data = randomNum;

        /* 上锁 */
        pthread_mutex_lock(&g_mutex);

        /* 头插 */
        newNode->next = g_head;
        g_head = newNode;

        printf("============ produce: %lu, %d\n", pthread_self(), newNode->data);

        /* 解锁 */
        pthread_mutex_unlock(&g_mutex);

        /* 通知阻塞的消费者线程，解除阻塞, 发信号 */
        pthread_cond_signal(&g_cond);

        /* 休息 */
        usleep(800000);
    }

    pthread_exit(NULL);
}

/* 消费者函数 */
void *handleConsume(void *arg)
{
    while (1)
    {
        /* 加锁 */
        pthread_mutex_lock(&g_mutex);

        /* 这里改成while, 响应不同情形下 */
        while (g_head == NULL)
        {
            /* 没有数据, 等待生产者发出信号, pthread_cong_signal() */
            pthread_cond_wait(&g_cond, &g_mutex);

            /* 当条件变量被满足的时候，解除阻塞，并且又会对互斥锁重新加上锁 */
        }

        // Node *eatNode = g_head;
        std::shared_ptr<Node> eatNode = g_head;
        g_head = g_head->next;

        /* 解锁 */
        pthread_mutex_unlock(&g_mutex);

        cout << "============ consume: " << pthread_self() << ", " << eatNode->data << endl;
    }

    pthread_exit(NULL);
}

int main()
{
    /* 随机数种子 */
    srand(static_cast<unsigned int>(time(nullptr)));

    /* 初始化锁 */
    pthread_mutex_init(&g_mutex, NULL);
    /* 初始化条件变量 */
    pthread_cond_init(&g_cond, NULL);

    /* 生产者线程 */
    pthread_t produce;
    int ret = pthread_create(&produce, NULL, handleProduce, NULL);
    if (ret == -1)
    {
        perror("produce_thread create error");
        _exit(-1);
    }

    /* 消费者线程 */
    pthread_t consume;
    ret = pthread_create(&consume, NULL, handleConsume, NULL);
    if (ret == -1)
    {
        perror("consume_thread create error");
        _exit(-1);
    }

    /* 阻塞回收子线程资源 */
    pthread_join(produce, NULL);
    pthread_join(consume, NULL);

    pthread_mutex_destroy(&g_mutex);

    pthread_cond_destroy(&g_cond);

    return 0;
}
```

### 4.线程池

- .h文件

```c++
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include <iostream>
#include <pthread.h>

struct Task_t
{
    /* 回调函数 */
    void *(*function)(void *arg);
    /* 回调函数的参数 */
    void *arg; 
};

class ThreadPool
{
public:
    ThreadPool(int minThread,int maxThread,int maxQueueCapacity); 
    ~ThreadPool();
public:
    /* 添加任务 */
    void addTask(void * (*function)(void *arg),void * arg);

    /* 队列的容量 */
    int m_queueCapacity;
    /* 队列任务个数 */
    int m_queueSize;
    /* 队列 */
    Task_t *m_queue;
    /* 队列头 */
    int m_queueFront;
    /* 队列尾*/
    int m_queueTail;

    /* 工作的线程 */
    pthread_t *m_threads;
    /* 管理者线程 */
    pthread_t manageThread;
    /* 忙碌的线程数 */
    int m_busyThreadNums;
    /* 最大的线程数 */
    int m_maxThreadsNums;
    /* 最小的线程数 */
    int m_minThreadsNums;
    /* 存活的线程数 */
    int m_aliveThreadNums;

public:

    /* 锁 */
    pthread_mutex_t m_mutex;
    /* 锁2 - 提升效率 - 减少竞争*/
    pthread_mutex_t m_busyMutex;
    /* 条件变量 */
    pthread_cond_t m_queueNotEmpty;
    pthread_cond_t m_queueNotFull;

    /* 要减少的线程数 */
    int m_exitNum;
};

#endif
```

- .cpp文件

```c++
#include "ThreadPool.h"
#include <cstring>
#include <unistd.h>
#include <signal.h>
#define DEFAULT_MINTHREADS 3
#define DEFAULT_MAXTHREADS 5
#define INCREASE_NUMS 2
#define DECREASE_NUMS 2
// /* 判断线程是否存活 */
// bool threadsIsAlive(pthread_t tid)
// {
//     int res = pthread_kill(tid, 0);
//     if (res == ESRCH)
//     {
//         return false;
//     }
//     return true;
// }

void exitThread(ThreadPool *pool, pthread_t tid)
{
    for (int idx = 0; idx < pool->m_maxThreadsNums; idx++)
    {
        if (pool->m_threads[idx] == tid)
        {
            pool->m_threads[idx] = 0;
            /* 线程退出 */
            pthread_exit(NULL);
        }
    }
}
void *workerThreadFunc(void *arg);

/* 管理者线程 */
void *manageThreadFunc(void *arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());
    /* 管理者线程要管理线程池 */
    ThreadPool *pool = (ThreadPool *)arg;
    while (true)
    {
        /* 休眠 */
        sleep(1);
        /* 共享数据 加锁 */
        /* 去除任务队列的任务个数 */
        /* 取出活跃的线程数 */
        pthread_mutex_lock(&(pool->m_mutex));
        int queueSize = pool->m_queueSize;
        int aliveThreadNums = pool->m_aliveThreadNums;
        pthread_mutex_unlock(&(pool->m_mutex));

        /* 取出忙碌的线程数 */
        pthread_mutex_lock(&(pool->m_busyMutex));
        int busyThreadNums = pool->m_busyThreadNums;
        pthread_mutex_unlock(&(pool->m_busyMutex));

        // printf("queueSize = %d,alive ThradsNums = %d,busyThreadsNums = %d\n", queueSize, aliveThreadNums, busyThreadNums);
        /* 任务队列的任务数大于活跃线程数  */
        int addCnt = 0;
        int ret = 0;

        if (queueSize > (aliveThreadNums - busyThreadNums) && aliveThreadNums < pool->m_maxThreadsNums)
        {

            pthread_mutex_lock(&(pool->m_mutex));

            for (int idx = 0; idx < pool->m_maxThreadsNums && addCnt < INCREASE_NUMS; idx++)
            {
                /* 探测*/
                if (pool->m_threads[idx] == 0)
                {
                    ret = pthread_create(&(pool->m_threads[idx]), NULL, workerThreadFunc, pool);
                    if (ret != 0)
                    {
                        perror("create error:");
                        throw std::runtime_error("thread_create error");
                    }
                    printf("%ld add join threadpool\n", pool->m_threads[idx]);

                    addCnt++;
                    /* 活着的线程 */
                    (pool->m_aliveThreadNums)++;
                }
            }
            pthread_mutex_unlock(&(pool->m_mutex));
        }
        /* 需要减少线程池的线程数 */
        /* 任务队列任务数 远小于 未工作的线程数 */
        if (queueSize < (aliveThreadNums - busyThreadNums) && aliveThreadNums > pool->m_minThreadsNums)
        {
            /* 减少线程 */
            pthread_mutex_lock(&(pool->m_mutex));

            pool->m_exitNum = DECREASE_NUMS;
            for (int idx = 0; idx < DECREASE_NUMS; idx++)
            {
                pthread_cond_signal(&(pool->m_queueNotEmpty));
            }
            pthread_mutex_unlock(&(pool->m_mutex));
        }
    }
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
            usleep(1000);
            /* 线程同步，等待，然后解锁 */
            pthread_cond_wait(&(pool->m_queueNotEmpty), &(pool->m_mutex));
            if (pool->m_exitNum > 0)
            {
                (pool->m_exitNum)--;

                if (pool->m_aliveThreadNums > pool->m_minThreadsNums)
                {
                    /* 存活的线程数 - 1*/

                    (pool->m_aliveThreadNums)--;
                    pthread_mutex_unlock(&(pool->m_mutex));

                    printf("%ld exit threadpool\n", pthread_self());

                    /* 线程退出 */
                    exitThread(pool, pthread_self());
                }
            }
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
        this->m_aliveThreadNums++;
    }
    /* 锁的初始化 */
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_busyMutex, NULL);

    /* 条件变量初始化 */
    pthread_cond_init(&m_queueNotEmpty, NULL);
    pthread_cond_init(&m_queueNotFull, NULL);

    /* 杀掉的线程数 */
    this->m_exitNum = 0;
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

```

# 网络

## 1.概念

OSI七层模型：物理层 数据链路层 网络层 传输层 会话层 表示层 应用层

IP地址族：ipv4 ipv6

PORT端口：ssh 22；http 80；https 443；MySQl 3306；

协议：TCP/UDP/IP http

## 服务器/客户端

### 1.服务器server

```c++
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <ctype.h>
#include <pthread.h>

void toLowercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = std::tolower(static_cast<unsigned char>(str[i]));
    }
}
#define SERVER_PORT 8080

void *handleClientInfo(void *arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());
    int connfd = *(int*)arg;
    char buffer[1024] = {0};
    int readBytes = 0;
    while(1)
    {
        readBytes = read(connfd, buffer, sizeof(buffer));
        if (readBytes > 0)
        {
            cout << "buffer:" << buffer << endl;
            for (int idx = 0; idx < readBytes; idx++)
            {
                buffer[idx] = toupper(buffer[idx]);
            }
            write(connfd, buffer, readBytes);
            // toLowercase(buffer);
        }
        else if (readBytes == 0)
        {
            /* 客户端断开连接 */
            cout << "readBytes = 0" << endl;
            break;
        }
        else if (readBytes < 0)
        {
            cout << "readBytes < 0" <<endl;
            break;
        }
    }
    /* 回收资源 */
    close(connfd);
    pthread_exit(NULL);

}
int main()
{
    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        _exit(-1);
    }
    cout << "sockfd:" << sockfd << endl;
    /* 设置端口复用 */
    int optVal = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (ret != 0)
    {
        perror("bind error:");
        _exit(-1);
    }
    /* 绑定IP和端口 */
    struct sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof(localAddress));
    /* 地址族 */
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(SERVER_PORT);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sockfd, reinterpret_cast<const sockaddr *>(&localAddress), sizeof(localAddress));
    if (ret != 0)
    {
        perror("bind error:");
        _exit(-1);
    }

    ret = listen(sockfd, 10);
    if (ret != 0)
    {
        perror("listen error:");
        _exit(-1);
    }

    int connfd = accept(sockfd, NULL, NULL);
    char buffer[1024] = {0};
    size_t readBytes = 0;

    int connfd = 0;
    while (1)
    {
        /* 接受到新的客户端的连接 - 就解除阻塞. */
        connfd = accept(sockfd, NULL, NULL);
        if (connfd == -1)
        {
            perror("accecp error:");
            _exit(-1);
        }
        /* 程序到达这个地方：单独开辟线程去处理客户端的数据通信 */
        /* 创建一个新的线程 */
        pthread_t tid = 0;
        ret = pthread_create(&tid, NULL, handleClientInfo, (void*) & connfd);
        if (ret != 0)
        {
            perror("thread create error:");
            _exit(-1);
        }
        /* 休眠1s:确保线程一定创建成功，并且接受到对应的connfd */
        sleep(1);
    }

    /* 释放资源 */
    /* 关闭文件描述符 */
    close(connfd);
    close(sockfd);
    cout << "client 断开连接!" << endl;
    return 0;
}
```

### 2.客户端

```c++
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080

int main()
{
    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        _exit(-1);
    }
    cout << "sockfd:" << sockfd << endl;

    /* 连接服务器 */
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);

    const char *ip = "192.168.23.128";
    inet_pton(AF_INET, ip, &(serverAddress.sin_addr.s_addr));

    int ret = connect(sockfd, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress));
    if (ret != 0)
    {
        perror("connect error:");
        _exit(-1);
    }
    char writebuffer[128] = {0};
    char readbuffer[128] = {0};
    while (1)
    {
        cout << "请输入要发送的信息:";
        cin >> writebuffer;
        write(sockfd, writebuffer, strlen(writebuffer) + 1);

        /* 休眠 */
        sleep(2);

        read(sockfd,readbuffer,sizeof(readbuffer));
        cout << "server:" << readbuffer << endl;
    }
    return 0;
}
```

# 数据库

## 1.sqlite3

```c++
#include "stdDataBase.h"
#include "sqliteDataBase.h"

sqliteDataBase::sqliteDataBase() : m_db(nullptr)
{
}

sqliteDataBase::~sqliteDataBase()
{
}

bool sqliteDataBase::setConnectDb(const string &dbFileName)
{
    /* 连接数据库 */
    int ret = sqlite3_open(dbFileName.c_str(), &m_db);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        return false;
    }
    return true;
}

bool sqliteDataBase::excuteSql(const string &sql)
{
    /* 执行sql语句 */
    int ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        return false;
    }
    return true;
}

vecResult sqliteDataBase::querySql(const string &sql)
{
    vector<vector<string>> Datas;

    char **result = NULL;
    int row = 0;
    int col = 0;
    int ret = sqlite3_get_table(m_db, sql.c_str(), &result, &row, &col, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3_get_table error" << sqlite3_errmsg(m_db) << endl;
        return Datas;
    }

    for (int i = 1; i <= row; i++)
    {
        vector<string> rowData;
        for (int j = 0; j < col; j++)
        {
            // cout << left << setw(16) << result[i * col + j] << " ";
            rowData.push_back(result[i * col + j]);
        }
        Datas.push_back(rowData);
    }
    return Datas;
}

void sqliteDataBase::disConnectDb(const string &dbFileName)
{
    sqlite3_close(m_db);
}

```

## 2.Mysql

```c++
#include "mysqlDataBase.h"
#include <iostream>

stdMysqlDataBase::stdMysqlDataBase() : m_sqlDB(nullptr), m_isConnected(false)
{
    m_sqlDB = mysql_init(nullptr);
    if (m_sqlDB == nullptr)
    {
        throw std::runtime_error("error initializing MySQL connection");
    }
}

stdMysqlDataBase::~stdMysqlDataBase()
{
}
bool stdMysqlDataBase::setConnectDb(const string &dbFileName)
{
    // /* 连接数据库 */
    // const char *host = "localhost";
    // const char *userName = "root";
    // const char *passwd = "1";
    // /* 3306 为mysql端口号 */
    // if (mysql_real_connect(m_sqlDB, host, userName, passwd, dbFileName.c_str(), 3306, NULL, 0) == NULL)
    // {
    //     cout << "file:" << __FILE__ << "line:" << __LINE__ << "connect mysql server error:" << mysql_error(m_sqlDB) << endl;
    //     return false;
    // }
    // cout << "连接成功" << endl;
    // return true;
#if 1
    /* 如果已经是连接的状态 */
    if (m_isConnected)
    {
        /* 已连接则需要先断开 */
        mysql_close(m_sqlDB);
        m_sqlDB = mysql_init(nullptr);
        if (m_sqlDB == nullptr)
        {
            throw std::runtime_error("Error reinitializing MySQL connection");
        }
    }

    if (m_sqlDB == nullptr)
    {
        m_sqlDB = mysql_init(nullptr);
        if (m_sqlDB == nullptr)
        {
            cerr << "Error initializing MySQL connection" << endl;
            return false;
        }
    }

    const char *host = "localhost";
    const char *userName = "root";
    const char *passwd = "1";
    unsigned int port = 3306;

    if (mysql_real_connect(m_sqlDB, host, userName, passwd, dbFileName.c_str(), port, nullptr, 0) == nullptr)
    {
        cerr << "File: " << __FILE__ << ", Line: " << __LINE__
             << ", Connect MySQL server error: " << mysql_error(m_sqlDB) << endl;
        mysql_close(m_sqlDB);
        m_sqlDB = nullptr;
        return false;
    }

    cout << "连接成功" << endl;
    m_isConnected = true;
    return true;
#endif
}

bool stdMysqlDataBase::excuteSql(const string &sql)
{
    /* 事务的开始 */
    if (mysql_query(m_sqlDB, "START TRANSACTION "))
    {
        cout << "START TRANSACTION" << mysql_error(m_sqlDB) << endl;
        return false;
    }

    if (mysql_query(m_sqlDB, sql.c_str()))
    {
        /* sql语句有问题 */
        cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(m_sqlDB) << endl;
        mysql_query(m_sqlDB, "ROLLBACK");
        return false;
    }
    else
    {
        /* sql语句没有问题 */
        /* 如果返回0，则表示没有行数被影响 */
        if (mysql_affected_rows(m_sqlDB) == 0)
        {
            cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(m_sqlDB) << endl;
            mysql_query(m_sqlDB, "ROLLBACK");
            return false;
        }
    }
    cout << "good sql..." << endl;

    /* 事务的提交 */
    if (mysql_query(m_sqlDB, "COMMIT"))
    {
        cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(m_sqlDB) << endl;
        mysql_query(m_sqlDB, "ROLLBACK");
        return false;
    }
    return true;
}

vecResult stdMysqlDataBase::querySql(const string &sql)
{
    vector<vector<string>> Datas;

    // 执行查询
    if (mysql_query(m_sqlDB, sql.c_str()) != 0)
    {
        cout << "mysql_query error: " << mysql_error(m_sqlDB) << endl;
        return Datas;
    }

    // 存储结果集
    MYSQL_RES *result = mysql_store_result(m_sqlDB);
    if (result == NULL)
    {
        cout << "mysql_store_result error: " << mysql_error(m_sqlDB) << endl;
        return Datas;
    }

    // 获取结果集的列数
    unsigned int cols = mysql_num_fields(result);

    // 获取结果集的行数
    unsigned long long rows = mysql_num_rows(result);

    // 遍历结果集
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        vector<string> rowData;
        for (unsigned int i = 0; i < cols; i++)
        {
            rowData.push_back(row[i] ? row[i] : "NULL");
        }
        Datas.push_back(rowData);
    }

    // 释放结果集
    mysql_free_result(result);
    return Datas;
}

void stdMysqlDataBase::disConnectDb(const string &dbFileName)
{
    mysql_close(m_sqlDB);
    m_isConnected = false;
}

MYSQL *stdMysqlDataBase::getConnection()
{
    if (m_sqlDB == nullptr)
    {
        throw std::runtime_error("MySQL connection is not initialized");
    }
    return m_sqlDB;
}
```

