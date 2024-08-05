#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "stdTcpServer.h"
#include "stdShared.h"
#include <cstring>
void interfaceMenu()
{
    cout << "1.注册" << endl;
    cout << "2.登陆" << endl;
    cout << "3.退出" << endl;
}
void registerFunc(StdTcpSocket &client)
{

    string username;
    cout << "请输入用户名:";
    fflush(stdout);
    cin >> username;

    string passwd;
    cout << "请输入密码:";
    fflush(stdout);
    cin >> passwd;

    Msg msg;
    memset(&msg,0,sizeof(msg));
    msg.type = REGISTER;
    strncpy(msg.name,username.c_str(),sizeof(msg.name) - 1);
    strncpy(msg.passwd,passwd.c_str(),sizeof(msg.passwd) - 1);
    /* 将用户名和密码发送到客户端 */
    client.sendMessage(&msg,sizeof(msg));
}
void loginFunc(StdTcpSocket &client)
{

    string username;
    cout << "请输入用户名:";
    fflush(stdout);
    cin >> username;

    string passwd;
    cout << "请输入密码:";
    fflush(stdout);
    cin >> passwd;

    Msg msg;
    memset(&msg,0,sizeof(msg));
    msg.type = LOGIN;
    strncpy(msg.name,username.c_str(),sizeof(msg.name) - 1);
    strncpy(msg.passwd,passwd.c_str(),sizeof(msg.passwd) - 1);
    /* 将用户名和密码发送到客户端 */
    client.sendMessage(&msg,sizeof(msg));
}
int main()
{
    /* 客户端 */
    StdTcpSocket client;
    /* 连接服务器 */
    const char *serverIp = "192.168.23.128";
    int ret = client.connctToServer(serverIp, 8080);
    if (ret != 0)
    {
        cout << "connectToserver failed" << endl;
        _exit(-1);
    }

    cout << "连接成功...\n";
    sleep(1);
    system("clear");
    int choice;
    while (1)
    {
        interfaceMenu();
        cout << "请输入你的选项:";
        cin >> choice;
        switch (choice)
        {
        case REGISTER:
            registerFunc(client);
            break;
        case LOGIN:
            loginFunc(client);
            break;
        case EXIT:
            exit(0);
            break;
        }
    }
    // string mes = "hello world";
    // client.sendMessage(mes);
    return 0;
}