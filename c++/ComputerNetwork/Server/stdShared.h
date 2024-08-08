#ifndef __STDSHARED_H__
#define __STDSHARED_H__

enum FUNCTION_OPTION
{
    REGISTER = 1,
    LOGIN,
    EXIT,
    ADDFRIEND,     /* 添加好友 */
    DELETEFRIEND,  /* 删除好友 */
    NEWGROUP,      /* 新建群 */
    EXITGROUP,     /* 退出群 */
    
};

struct Msg
{
    FUNCTION_OPTION type;
    char name[20];
    char passwd[20];
};

enum REPLY_CODE
{
    LOGIN_SUCCESS,      //登陆成功
    LOGIN_PASSWD_ERROR, //登陆密码错误
    LOGIN_NOUSER,       //没有该用户
    REGISTER_SUCCESS,   //注册成功
    REGISTER_USEREXIST  //用户已经存在
};

struct ReplyMsg
{
    FUNCTION_OPTION type;   //操作类型
    REPLY_CODE statue_code; // 回复的状态码
};


#endif