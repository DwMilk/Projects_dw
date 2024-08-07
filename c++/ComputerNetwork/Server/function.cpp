#include "function.h"
#include <string>
using namespace std;
Function::Function(const StdTcpSocketPtr & clientInfo)
{
    m_clientInfo = clientInfo;
}

Function::~Function()
{
}

void Function::handleRegisterInfo(const Msg & msg)
{
    std::cout << "username:" << msg.name << std::endl;
    std::cout << "passwd:" << msg.passwd << std::endl;
    string replyInfo = "注册成功";
    m_clientInfo->sendMessage(replyInfo);
}

void Function::handleLoginInfo(const Msg & msg)
{
    std::cout << "username:" << msg.name << std::endl;
    std::cout << "passwd:" << msg.passwd << std::endl;
    string replyInfo = "登陆失败，密码不正确.";
    m_clientInfo->sendMessage(replyInfo);
#if 0
    /* 判断用户名是否已经存在 */
    if (userIsExist(username) == false)
    {
        /* 程序进入，说明用户名不存在*/

        /* 将信息发送到客户端 */
    }
    else
    {
        /* 如果用户名存在，判断密码 */
        userIsMatch(username, passwd);
        /* 判断用户是否已经登陆 */
        userIsOnline(username);
    }
#endif
    
}

void Function::handleAddFrienfInfo(const Msg & msg)
{
}

void Function::handleDeleteFrienfInfo(const Msg & msg)
{
}

void Function::handleNewGroup(const Msg & msg)
{
}

void Function::handleExitGroupInfo(const Msg & msg)
{
}

bool Function::userIsExist(const char *username)
{
    return true;
}

bool Function::userIsMatch(const char *username, const char *passwd)
{
    return true;
}

bool Function::userIsOnline(const char *username)
{
    return true;
}
