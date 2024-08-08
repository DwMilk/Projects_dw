#ifndef __STDTCPSERVER_H_
#define __STDTCPSERVER_H_

#include <memory>
#include <iostream>
#include <string>
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

struct StdTcpSocketPrivate
{
    /* 通信文件描述符 */
    int connfd;
    /* 通信是否连接成功 */
    bool m_connected;
};

/* 通信类 */
class StdTcpSocket
{
public:
    /* 构造函数 */
    StdTcpSocket();
    /* 析构函数 */
    ~StdTcpSocket();

public:
    /* 连接服务器 */
    int connectToServer(const char *ip, int port);
    /* 是否连接成功 */
    bool isConnected();
    /* 发送信息 */
    int sendMessage(std::string &sendMsg);
    /* 发送信息2 */
    int sendMessage(const void *sendMsg, size_t n);

    /* 接受消息 */
    int recvMessage(std::string &recvMsg);
    /* 接受消息2 */
    int recvMessage(void *buf, size_t n);

    /* 得到属性 */
    StdTcpSocketPrivate *getSockAttr();

private:
    StdTcpSocketPrivate *m_sockAttr;
};

struct stdTcpServerPrivate
{
    /* 通信的文件描述符 */
    int sockfd;
    /* 是否正在监听 */
    bool m_isRunning;
};

class StdTcpServer
{
public:
    /* 构造函数 */
    StdTcpServer();

    /* 析构函数 */
    ~StdTcpServer();

public:
    /* 设置监听 */
    bool setListen(int port);

    /* 接受连接 */
    std::shared_ptr<StdTcpSocket> getClientSock();

private:
    /* 端口 */
    int m_port;
    /* 服务器的属性 */
    std::shared_ptr<stdTcpServerPrivate> m_tcpAttr;
};

#endif