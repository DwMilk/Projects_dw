#include "function.h"

void Function::handleRegisterInfo(const char *username, const char *passwd)
{
}

void Function::handleLoginInfo(const char *username, const char *passwd)
{
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
