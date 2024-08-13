#include "chatroom.h"
#include <iostream>
#include <cstring>
using namespace std;
ChatRoom::ChatRoom(const StdTcpSocketPtr &clientInfo) : m_clientSock(clientInfo)
{
}

ChatRoom::~ChatRoom()
{
}

void ChatRoom::ChatEntry()
{
    ChatMenu();

    int choice = 0;
    std::cin >> choice;

    /* addFriend的默认偏移值是3 */
    choice += m_offset;
    switch (choice)
    {
    case ADDFRIEND:
        addFriend();
        break;
    case DELETEFRIEND:
        delFriend();
        break;
    default:
        break;
    }
}

/* 聊天菜单 */
void ChatRoom::ChatMenu()
{
    cleanScreen();
    std::cout << " 1.添加好友 " << std::endl;
    std::cout << " 2.删除好友 " << std::endl;
    std::cout << " 3.好友聊天 " << std::endl;
    std::cout << " 4.好友列表 " << std::endl;

    std::cout << " 5.黑名单 " << std::endl;
    std::cout << " 6.创建群聊 " << std::endl;
    std::cout << " 7.邀请群聊 " << std::endl;
    std::cout << " 8.加入群聊 " << std::endl;
    std::cout << " 9.退出群聊 " << std::endl;
}

void ChatRoom::addFriend()
{
    // cout << " addFriend" << endl;
    string addName;
    cout << "请输入联系人的用户名：" << endl;
    cin >> addName;

    Msg msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = ADDFRIEND;
    strncpy(msg.FriName, addName.c_str(), sizeof(msg.FriName) - 1);

    /* 发送信息给服务器 */
    m_clientSock->sendMessage((const void *)&msg, sizeof(msg));
}

void ChatRoom::delFriend()
{
    string delName;
    cout << "请输入删除联系人的用户名:" << endl;
    cin >> delName;

    Msg msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = DELETEFRIEND;
    strncpy(msg.FriName, delName.c_str(), sizeof(msg.FriName) - 1);

    /* 发送信息给服务器 */
    m_clientSock->sendMessage((const void *)&msg, sizeof(msg));
}

void ChatRoom::startChatWithFriend()
{
    string targteName;
    cout << "请输入你想聊天的用户:" << endl;
    cin >> targteName;

    Msg msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = FRIENDCHAT;
    strncpy(msg.FriName, targteName.c_str(), sizeof(msg.FriName) - 1);

    /* 发送信息给服务器 */
    m_clientSock->sendMessage((const void *)&msg, sizeof(msg));
}

void ChatRoom::openFriendList()
{
    string FrinedListl;
    cout << "好友界面:" << endl;
    Msg msg;
    memset(&msg,0,sizeof(msg));
    msg.type = FRIENDLIST;
    strncpy(msg.FriName,FrinedListl.c_str(),sizeof(msg.FriName) - 1);

    /* 发送信息给服务器 */
    m_clientSock->sendMessage((const void *)& msg,sizeof(msg));
}

void ChatRoom::newGroupChat()
{
    string newGroupChat;
    cout << "创建一个新的群组聊天" << endl;
    Msg msg;
    
    memset(&msg,0,sizeof(msg));
    msg.type = NEWGROUP;
    strncpy(msg.GrpNam,newGroupChat.c_str(),sizeof(msg.GrpNam) - 1);

    /* 发送信息给服务器 */
    m_clientSock->sendMessage((const void *)& msg,sizeof(msg));
}

void ChatRoom::quitGroupChat()
{

}

void ChatRoom::joinNewGroupChat()
{
}

void ChatRoom::inviteToGroupChat()
{
}

void ChatRoom::openGroupChatWindow()
{
}
