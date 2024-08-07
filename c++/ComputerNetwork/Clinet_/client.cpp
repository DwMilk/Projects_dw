#include <iostream>
using namespace std;

#include "stdTcpServer.h"
#include "stdShared.h"
#include <unistd.h>
#include <cstring>

/* 界面菜单 */
void interfacemenu()
{
    cout << "1、注册" << endl;
    cout << "2、登录" << endl;
    cout << "3、退出" << endl;
}

/* 注册功能 */
void registerFunc(StdTcpSocket &client)
{
    system("clear");

    string username;
    cout << "请输入用户名:";
    fflush(stdout);
    cin >> username;

    string passwd;
    cout << "请输入账号密码:";
    fflush(stdout);
    cin >> passwd;

    Msg msg;
    memset(&msg, 0, sizeof(msg));

    msg.type = REGISTER;
    strncpy(msg.name, username.c_str(), sizeof(msg.name) - 1);
    strncpy(msg.passwd, passwd.c_str(), sizeof(msg.passwd) - 1);

    /* 将用户名和密码发送给服务器 */
    client.sendMessage(&msg, sizeof(msg));

    char buffer[128] = { 0 };
    client.recvMessage(buffer, sizeof(buffer));
    cout << "buffer:" << buffer << endl;
}

/* 登录功能 */
void loginFunc(StdTcpSocket &client)
{
    system("clear");

    string username;
    cout << "请输入用户名:";
    fflush(stdout);
    cin >> username;

    string passwd;
    cout << "请输入账号密码:";
    fflush(stdout);
    cin >> passwd;

    Msg msg;
    memset(&msg, 0, sizeof(msg));

    msg.type = LOGIN;
    strncpy(msg.name, username.c_str(), sizeof(msg.name) - 1);
    strncpy(msg.passwd, passwd.c_str(), sizeof(msg.passwd) - 1);

    /* 将用户名和密码发送给服务器 */
    client.sendMessage(&msg, sizeof(msg)); 

    char buffer[128] = { 0 };
    client.recvMessage(buffer, sizeof(buffer));
    cout << "buffer:" << buffer << endl;

}

int main()
{
    /* 客户端 */
    StdTcpSocket client;

    /* 连接服务器 */
    const char * serverIp = "192.168.23.133";
    int ret = client.connectToServer(serverIp, 8080);
    if (ret != 0)
    {
        cout << "connectToServer failed" << endl;
        _exit(-1);
    }
    
    cout << "连接成功...\n";

    sleep(1);
    system("clear");

    int choice;
    while (1)
    {
        interfacemenu();
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
    
    return 0;
}