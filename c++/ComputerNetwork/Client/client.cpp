#include <iostream>
using namespace std;
#include "stdTcpServer.h"
#include <unistd.h>
#include "stdShared.h"
#include <cstring>
#include "chatroom.h"
/* 界面菜单 */
void interaceMenu()
{
    // cleanScreen();
    cout << "1. 注册" << endl;
    cout << "2. 登录" << endl;
    cout << "3. 退出" << endl;
}

/* 注册功能 */
void registerFunc(StdTcpSocketPtr &client)
{
    string username, passwd, type;
    type = "1";
    cout << "请输入注册的用户名: ";
    cin >> username;
    cout << "请输入你的注册密码: ";
    cin >> passwd;
    string message = type + " " + username + " " + passwd;

    /* 发送信息给服务器 */
    Msg msg;
    memset(&msg, 0, sizeof(msg));

    msg.type = REGISTER;
    strncpy(msg.Usrname, username.c_str(), sizeof(msg.Usrname) - 1);
    strncpy(msg.passwd, passwd.c_str(), sizeof(msg.passwd) - 1);

    /* 将用户和密码发送给服务器 */
    client->sendMessage(&msg, sizeof(msg));
    ReplyMsg recvMsg;
    client->receiveMessage((void *)&recvMsg, sizeof(recvMsg));
    if (recvMsg.statue_code == REGISTER_SUCCESS)
    {
        cout << "注册成功 " << endl;
    }
    else if (recvMsg.statue_code == REGISTER_USEREXIST)
    {
        cout << "重复注册，用户已经存在 " << endl;
    }
}

/* 登录功能 */
void loginFunc(StdTcpSocketPtr &client)
{
    string username, passwd, type;
    type = "2";
    cout << "请输入登录用户名: ";
    cin >> username;
    cout << "请输入你的密码: ";
    cin >> passwd;
    string message = type + " " + username + " " + passwd;

    /* 发送信息给服务器 */
    Msg msg;
    memset(&msg, 0, sizeof(msg));

    msg.type = LOGIN;
    strncpy(msg.Usrname, username.c_str(), sizeof(msg.Usrname) - 1);
    strncpy(msg.passwd, passwd.c_str(), sizeof(msg.passwd) - 1);

    /* 将用户和密码发送给服务器 */
    client->sendMessage(&msg, sizeof(msg));
    ReplyMsg recvMsg;
    client->receiveMessage((void *)&recvMsg, sizeof(recvMsg));
    if (recvMsg.statue_code == LOGIN_SUCCESS)
    {
        cout << "登陆成功 " << endl;
        sleep(2);
        ChatRoom chatroom(client);
        chatroom.ChatEntry();
        
    }
    else if (recvMsg.statue_code == LOGIN_PASSWD_ERROR)
    {
        cout << " 密码错误 " << endl;
    }
    else if (recvMsg.statue_code == LOGIN_NOUSER)
    {
        cout << "没有该用户 " << endl;
    }
}

int main()
{
    /* 客户端 */
    StdTcpSocketPtr client = std::make_shared<StdTcpSocket>();

    /* 连接服务器 */
    const char *serverIp = "192.168.23.134";
    int ret = client->connctToServer(serverIp, 8080);
    if (ret != 0)
    {
        cout << "connectToServer error" << endl;
        _exit(-1);
    }

    cout << "连接成功... " << endl;
    sleep(1);
    system("clear");

    int choice;
    while (1)
    {
        interaceMenu();
        cout << "请输入你的选项: ";
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

        default:
            break;
        }
    }

    return 0;
}