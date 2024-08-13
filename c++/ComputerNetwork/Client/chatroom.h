#ifndef __CHATROOM_H__
#define __CHATROOM_H__
#include "stdTcpServer.h"
#include "stdShared.h"

class ChatRoom
{
public:
    ChatRoom(const StdTcpSocketPtr & clientInfo);
    ~ChatRoom();
public:
    /* 聊天室入口 */
    void ChatEntry();




private:
    /* 聊天菜单 */
    void ChatMenu();

    /* 好友业务*/
    /* 添加好友 */
    void addFriend();

    /* 删除好友 */
    void delFriend();

    /* 好友聊天 */
    void startChatWithFriend();

    /* 打开好友列表 */
    void openFriendList();

    /* 群聊业务 */
    /* 新建群聊 */
    void newGroupChat();

    /* 退出群聊 */
    void quitGroupChat();

    /* 加入群聊 */
    void joinNewGroupChat();

    /* 邀请群聊 */
    void inviteToGroupChat();

    /* 群聊聊天界面 */
    void openGroupChatWindow();

private:
    StdTcpSocketPtr m_clientSock;

    /* 偏移 */
    static const int m_offset = 3;
};

#endif