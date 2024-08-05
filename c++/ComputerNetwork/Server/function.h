#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <iostream>
#include <string>

class Function
{
public:
    /* 构造函数 */
    Function();
    /* 析构函数 */
    ~Function();

public:
    /* 处理注册 */
    void handleRegisterInfo(const char *username, const char *passwd);

    /* 处理登陆 */
    void handleLoginInfo(const char *username, const char *passwd);
private:
    /* 判断用户名是否存在 */
    bool userIsExist(const char *username);
    /* 判断密码是否匹配 */
    bool userIsMatch(const char *username, const char *passwd);
    /* 用户是否在线 */
    bool userIsOnline(const char * username);

};

#endif