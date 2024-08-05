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