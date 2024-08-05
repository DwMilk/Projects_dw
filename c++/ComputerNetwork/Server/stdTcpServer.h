#ifndef __STDTCPSERVER_H__
#define __STDTCPSERVER_H__
#include <memory>
#include <string>
struct StdTcpSocketPrivate
{
    /* 通信句柄 */
    int connfd;
    /* 通信是否建立成功 */
    bool m_connected;
};



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
    bool isConnected();
    void SetConnection(int sockfd,bool m_isRunning);
    /* 发送信息 */
    int sendMessage(std::string & sendMsg);
    int sendMessage(const char * sendMsg,size_t n);

    /* 接收信息 */
    int receiveMessage(std::string & receiveMsg);
    int receiveMessage(void * buf,size_t n);

private:
    std::shared_ptr<StdTcpSocketPrivate> m_sockAttr;
};
struct StdTcpServerPrivate
{
    /* 监听句柄 */
    int sockfd;
    /* 服务器是否正在监听 */
    bool m_isRunning;
};

class StdTcpServer
{
public:
    StdTcpServer();
    ~StdTcpServer();

public:
    /* 设置监听 */
    bool setListen(int port);

    /* 接收客户端连接 */
    int getClientSock();

private:
    /* 端口 */
    int m_port;
    /* 服务器的属性 */
    // StdTcpServerPrivate * m_tcpAttr;
    std::shared_ptr<StdTcpServerPrivate> m_tcpAttr;
};

#endif