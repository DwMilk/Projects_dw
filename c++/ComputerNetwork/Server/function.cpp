#include "function.h"
#include <string>
#include <unistd.h>
#include <cstring>
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

#if 1   
    ReplyMsg responseMsg;
    responseMsg.type = REGISTER;
    /* 判断用户是否存在 */
    if(userIsExist(msg.name) == true)
    {
        responseMsg.statue_code = REGISTER_USEREXIST;
    }
    else
    {
        responseMsg.statue_code = REGISTER_SUCCESS;
        /* todo... */
    }
    /* 将消息发送会客户端 */
    this->m_clientInfo->sendMessage(static_cast<const void *>(&responseMsg),sizeof(responseMsg));
#endif 
}

void Function::handleLoginInfo(const Msg & msg)
{
    std::cout << "username:" << msg.name << std::endl;
    std::cout << "passwd:" << msg.passwd << std::endl;



    string username(msg.name);
    string passwd(msg.passwd);
    ReplyMsg responseMsg;
    memset(&responseMsg,0,sizeof(responseMsg));

    responseMsg.type = LOGIN;
    if(username == "zhangsan" )
    {
        if(passwd == "123456")
        {
            /* 用户名和密码匹配 - 登陆成功 */
            responseMsg.statue_code =LOGIN_SUCCESS;
        }
        else
        {
            /* 用户名不匹配 - 登录失败 */
            responseMsg.statue_code = LOGIN_PASSWD_ERROR;
        }
    }
    else
    {
        /* 没有该用户 */
        responseMsg.statue_code = LOGIN_NOUSER;
        
    }
    /* 发送信息给客户端 */
    m_clientInfo->sendMessage(static_cast<const void *> (&responseMsg),sizeof(responseMsg));

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
