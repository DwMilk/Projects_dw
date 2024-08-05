#include <iostream>

using namespace std;
#include "stdTcpServer.h"
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <sstream>
#include "stdShared.h"
#include "function.h"
void *handleClinetInfo(void *arg)
{
    std::shared_ptr<StdTcpSocket> *pClientInfo = static_cast<std::shared_ptr<StdTcpSocket> *>(arg);
    std::shared_ptr<StdTcpSocket> clientSocket = *pClientInfo;

    // auto clientSocket = (static_cast<std::shared_ptr<StdTcpSocket> *>(arg)).get();
    // char buffer[1024] = {0};
    Msg msg;
    /* 清空脏数据 */
    memset(&msg,0,sizeof(msg));
    while (1)
    {
        int readBytes = clientSocket->receiveMessage(&msg, sizeof(msg));
        if (readBytes <= 0)
        {
            break;
        }
        cout << "msg.type:" << msg.type << endl;
        if(msg.type == REGISTER)
        {
            handleRegisterInfo(msg.name,msg.passwd);
        }
        else if(msg.type == LOGIN)
        {
            handleLoginInfo(msg.name,msg.passwd);
        }
        cout << "msg.name:" << msg.name << endl;
        cout << "msg.passwd:" << msg.passwd << endl;
        // for (int idx = 0; idx < readBytes; idx++)
        // {
        //     buffer[idx] = toupper(buffer[idx]);
        // }
        // string msg(buffer);
        // char space = ':';
        // /* 找到函数在字符串的位置 */
        // size_t size = msg.find(space);
        // cout << "size:" << size << endl;
        // // cout << "msg[0]:" << msg[0] << endl;
        // if(msg[0] == 'R')
        // {
        //     /* 注册逻辑 */
        // }
        // if(msg[0] == 'L')
        // {
        //     string username;
        //     string passwd;

        //     /* 跳过R:*/
        //     string userPassPart = msg.substr(2);
        //     std::istringstream iss(userPassPart);
        //     iss >> username >> passwd;

        // }

        // int sentBytes = clientSocket->sendMessage(msg.passwd, readBytes);
        // if (sentBytes <= 0)
        // {
        //     break;
        // }
        memset(&msg, 0, sizeof(msg));
    }
    return NULL;
}
int main()
{
    StdTcpServer server;
    bool res = server.setListen(8080);
    if (res == false)
    {
        cout << "listen error" << endl;
        _exit(-1);
    }
    cout << "server listening" << endl;
    int ret = 0;
    while (1)
    {
        int clientSock = server.getClientSock();
        if (clientSock == -1)
        {
            perror("access error:");
            continue;
        }
        // 为每个客户端创建一个新的 StdTcpSocket 对象
        auto clientSocket = make_shared<StdTcpSocket>();
        clientSocket->SetConnection(clientSock, true);

        pthread_t tid;
        ret = pthread_create(&tid, NULL, handleClinetInfo, &clientSocket);
        if (ret != 0)
        {
            perror("thread create error :");
            _exit(-1);
        }
        pthread_detach(tid);
        sleep(1);
    }
    return 0;
}