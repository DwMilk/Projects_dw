#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <iostream>
#include <string>
#include "stdTcpServer.h"
#include "stdShared.h"
class Function
{
public:
    /* 构造函数 */
    Function(const StdTcpSocketPtr & clientInfo);
    /* 析构函数 */
    ~Function();

public:
    /* 处理注册 */
    void handleRegisterInfo(const Msg & msg);

    /* 处理登陆 */
    void handleLoginInfo(const Msg & msg);

    void handleAddFrienfInfo(const Msg & msg);

    void handleDeleteFrienfInfo(const Msg & msg);

    void handleNewGroup(const Msg & msg);

    void handleExitGroupInfo(const Msg & msg);

private:
    /* 判断用户名是否存在 */
    bool userIsExist(const char *username);
    /* 判断密码是否匹配 */
    bool userIsMatch(const char *username, const char *passwd);
    /* 用户是否在线 */
    bool userIsOnline(const char *username);

private:
    StdTcpSocketPtr m_clientInfo;
};

#endif