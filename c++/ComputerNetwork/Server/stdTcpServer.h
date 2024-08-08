#ifndef __STDTCPSERVER_H__
#define __STDTCPSERVER_H__
#include <memory>
#include <string>


/* 前置声明 */
struct StdTcpSocketPrivate;
struct StdTcpServerPrivate;

/* 通信类 */
class StdTcpSocket
{
public:
    StdTcpSocket();
    ~StdTcpSocket();
public:
    /* 连接服务器 */
    int connctToServer(const char * ip,int port);
    /* 是否连接成功 */
    bool isConnected() ;
    void SetConnection(int sockfd,bool m_isRunning);
    /* 发送信息 */
    int sendMessage(std::string & sendMsg);
    int sendMessage(const void * sendMsg,size_t n);

    /* 接收信息 */
    int receiveMessage(std::string & receiveMsg);
    int receiveMessage(void * buf,size_t n);

    /* 设置属性 */
    StdTcpSocketPrivate * getSockAttr() ;
private:
    /* 智能指针 */
    // std::shared_ptr<StdTcpSocketPrivate> m_sockAttr;
    std::unique_ptr<StdTcpSocketPrivate> m_sockAttr;
};


/* 智能指针 */
using StdTcpSocketPtr = std::shared_ptr<StdTcpSocket>;

class StdTcpServer
{
public:
    StdTcpServer();
    ~StdTcpServer();

public:
    /* 设置监听 */
    bool setListen(int port);

    /* 接收客户端连接 */
    std::shared_ptr<StdTcpSocket> getClientSock();

private:
    /* 端口 */
    int m_port;
    /* 服务器的属性 */
    std::unique_ptr<StdTcpServerPrivate> m_tcpAttr;
};

#endif