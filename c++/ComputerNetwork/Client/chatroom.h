#ifndef __CHATROOM_H__
#define __CHATROOM_H__
#include "stdTcpServer.h"

class ChatRoom
{
public:
    ChatRoom(const StdTcpSocket & clientSock);
    ~ChatRoom();
public:
    /* 聊天菜单 */
    void ChatMenu();
private:    
};





#endif