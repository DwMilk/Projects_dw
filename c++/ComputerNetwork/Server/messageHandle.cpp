#include "messageHandle.h"
using namespace std;
/* 构造函数 */
MessageHandle::MessageHandle(const StdTcpSocketPtr & clientInfo) : m_function(clientInfo)
{
    m_handles[REGISTER] = [this](const Msg & msg){ m_function.handleRegisterInfo(msg);};
    m_handles[LOGIN] = [this](const Msg & msg){ m_function.handleLoginInfo(msg);};
}
/* 析构函数 */
MessageHandle::~MessageHandle()
{
}

/* 处理信息 */
void MessageHandle::handleMessage(const Msg &msg)
{
#if 0
    if (msg.type == REGISTER)
    {
        m_function.handleRegisterInfo(msg);
    }
    else if (msg.type == LOGIN)
    {
        m_function.handleLoginInfo(msg);
    }
    else if (msg.type == ADDFRIEND)
    {
        m_function.handleAddFrienfInfo();
    }
    else if (msg.type == DELETEFRIEND)
    {
        m_function.handleDeleteFrienfInfo();
    }
    else if (msg.type == NEWGROUP)
    {
        m_function.handleNewGroup();
    }
    else if (msg.type == EXITGROUP)
    {
        m_function.handleExitGroupInfo();
    }
#endif
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
