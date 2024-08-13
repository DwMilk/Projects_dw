#include "messageHandle.h"
using namespace std;
/* 构造函数 */
MessageHandle::MessageHandle(const StdTcpSocketPtr & clientInfo) : m_function(clientInfo)
{
    /* 注册业务 */
    m_handles[REGISTER] = [this](const Msg & msg){ m_function.handleRegisterInfo(msg);};
    m_handles[LOGIN] = [this](const Msg & msg){ m_function.handleLoginInfo(msg);};

    /* 好友业务 */
    m_handles[ADDFRIEND] = [this](const Msg & msg) { m_function.handleAddFrienfInfo(msg);};
    m_handles[DELETEFRIEND] = [this](const Msg & msg) { m_function.handleDeleteFrienfInfo(msg);};
    m_handles[FRIENDCHAT] = [this](const Msg & msg) { m_function.handleFriendChatInfo(msg);};
    m_handles[FRIENDLIST] = [this](const Msg & msg){ m_function.handleFriendListInfo(msg);};

    /* 群组业务 */
    m_handles[NEWGROUP] = [this](const Msg & msg) { m_function.handleNewGroupInfo(msg);};
    m_handles[EXITGROUP] = [this](const Msg & msg){ m_function.handleExitGroupInfo(msg);};
    m_handles[JOINGROUP] = [this](const Msg & msg){ m_function.handleJoinGroupInfo(msg);};
    m_handles[INVITEGROUP] = [this](const Msg & msg){ m_function.handleInviteInfo(msg);};
    m_handles[CHATGROUP] = [this](const Msg & msg){ m_function.handleGroupChatInfo(msg);};
}
/* 析构函数 */
MessageHandle::~MessageHandle()
{
}

/* 处理信息 */
void MessageHandle::handleMessage(const Msg &msg)
{
    auto iter = m_handles.find(msg.type);
    if(iter != m_handles.end())
    {
        /* 执行回调函数 */
        iter->second(msg);
    }
    else
    {
        /* 处理未知的消息类型 */
        cout << "unknown message type."<< endl;
    }
}
