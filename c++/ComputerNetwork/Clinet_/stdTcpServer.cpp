#include "stdTcpServer.h"

#include <iostream>
using namespace std;

#include <sys/types.h>         
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>  //sockaddr_in的头文件
#include <cctype>
#include <arpa/inet.h> //  inet_pton 的头文件

StdTcpSocket::StdTcpSocket() : m_sockAttr(new StdTcpSocketPrivate)
{
    /* 通信句柄 */
    m_sockAttr->connfd = -1;
    /* 通信是否建立 */
    m_sockAttr->m_connected = false;
}

StdTcpSocket::~StdTcpSocket()
{
    close(m_sockAttr->connfd);
    m_sockAttr->connfd = -1;
    m_sockAttr->m_connected = false;

    if (m_sockAttr)
    {
        delete m_sockAttr;
        m_sockAttr = nullptr;
    }
}

int StdTcpSocket::connectToServer(const char *ip, int port)
{
    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        return -1;
    }
    m_sockAttr->connfd = sockfd;

    /* 连接服务器 */
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET; // 服务器的IPv4
    serverAddress.sin_port = htons(port); //  端口8080 使用htons()函数将主机字节序转换为网络字节序。

    inet_pton(AF_INET, ip, &(serverAddress.sin_addr.s_addr));

    // 连接服务器的ip地址
    int ret = connect(sockfd, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress));
    if (ret != 0)
    {
        perror("connect error");
        return -1;
    }

    m_sockAttr->m_connected = true;

    return 0;
}

bool StdTcpSocket::isConnected()
{
    return m_sockAttr->m_connected;
}

int StdTcpSocket::sendMessage(std::string &sendMsg)
{
    return sendMessage(sendMsg.c_str(), sendMsg.size());
}

int StdTcpSocket::sendMessage(const void *sendMsg, size_t n)
{
    int writeBytes = write(m_sockAttr->connfd, sendMsg, n);
    return writeBytes;
}

int StdTcpSocket::recvMessage(std::string &recvMsg)
{
    /* todo... */
    return 0;
}

int StdTcpSocket::recvMessage(void *buf, size_t n)
{
    int readBytes = read(m_sockAttr->connfd, buf, n);
    return readBytes;
}

StdTcpSocketPrivate *StdTcpSocket::getSockAttr()
{
    return m_sockAttr;
}

StdTcpServer::StdTcpServer() : m_tcpAttr(new StdTcpServerPrivate)
{
    /* 监听套接字 */
    m_tcpAttr->sockfd = -1;
    /* 是否监听 */
    m_tcpAttr->m_isRunning = false;
}

StdTcpServer::~StdTcpServer()
{
    /* 关闭句柄 */
    close(m_tcpAttr->sockfd);

    {
        m_tcpAttr->sockfd = -1;
        m_tcpAttr->m_isRunning = false;
    }

    /* 释放内存 */
    if (m_tcpAttr)
    {
        delete m_tcpAttr;
        m_tcpAttr = nullptr;
    }
}

bool StdTcpServer::setListen(int port)
{
    /* 类内部维护端口信息 */
    this->m_port = port;

    /* 创建套接字 - 用来创建一个监听的文件描述符 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("socket error");
        return false;
    }

    // cout << "sockfd:" << sockfd <<endl;  // 3

    /* 设置监听的套接字 */
    m_tcpAttr->sockfd = sockfd;

    /* 设置端口复用 */
    int optVal = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (ret != 0)
    {
        perror("setsockopt error");
        return false;
    }

    /* 绑定IP和端口 使用bind()函数将套接字与特定的IP地址和端口绑定 */  
    struct sockaddr_in localAddress; // 这里使用的是sockaddr_in，是因为struct sockaddr_in 提供了一个更加具体和易于使用的方式来处理 IPv4 地址。它包含了专门用于存储 IPv4 地址和端口号的字段，这使得设置和访问这些信息变得更加直观和方便。
    memset(&localAddress, 0, sizeof(localAddress));

    /* 地址族 */
    localAddress.sin_family = AF_INET; // 地址族，使用IPv4。
    localAddress.sin_port = htons(m_port); // 端口8080 使用htons()函数将主机字节序转换为网络字节序。
    localAddress.sin_addr.s_addr = htons(INADDR_ANY);  // ip地址， 使用INADDR_ANY表示接受任何IP地址。

    /* 绑定 */  
    ret = bind(sockfd, reinterpret_cast<const sockaddr *>(&localAddress), sizeof(localAddress)); // 这里用reinterpret_cast<const sockaddr *>(&localAddress)强转，是因为bind本身是要用sockaddr，而我们为了方便使用的是sockaddr_in，所以要强转
    if (ret != 0)
    {
        perror("bind error");
        return false;
    }

    /* 给监听的套接字设置监听，等待客户端连接 */
    ret = listen(sockfd, 1);
    if (ret != 0)
    {
        perror("listen error");
        return false;
    }

    /* 设置状态为:正在监听 */
    m_tcpAttr->m_isRunning = true;

    return true;
}

StdTcpSocketPtr StdTcpServer::getClientSock()
{
    int clientConnfd = accept(m_tcpAttr->sockfd, NULL, NULL);
    if (clientConnfd == -1)
    {
        perror("accept error");
        return StdTcpSocketPtr();
    }

    /* 程序到这个地方，就说明有客户端连接 */
    cout << "clientConnfd:" << clientConnfd <<endl;

    /* 通信类 */
    StdTcpSocketPtr ptr(new StdTcpSocket);
    
    /* 套接字 */
    ptr->getSockAttr()->connfd = clientConnfd;
    ptr->getSockAttr()->m_connected = true;

    return ptr;
}

