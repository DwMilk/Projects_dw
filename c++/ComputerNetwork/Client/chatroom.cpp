#include "chatroom.h"

ChatRoom::ChatRoom(const StdTcpSocket & clientSock) : m_client(clientSock)
{
}

ChatRoom::~ChatRoom()
{
}

/* 聊天菜单 */
void ChatRoom::ChatMenu()
{

}
