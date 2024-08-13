#ifndef __STDSHARED_H__
#define __STDSHARED_H__
#include <iostream>
using namespace std;


enum FUNCTION_OPTION
{
    /* 如果不标注1 默认从0开始偏移*/
    REGISTER = 1,
    LOGIN,
    EXIT,
    ADDFRIEND,     /* 添加好友 */
    DELETEFRIEND,  /* 删除好友 */
    FRIENDCHAT,    /* 好友聊天 */
    FRIENDLIST,    /* 好友列表 */

    NEWGROUP,      /* 新建群聊 */
    EXITGROUP,     /* 退出群聊 */
    JOINGROUP,     /* 加入群聊*/
    INVITEGROUP,   /* 邀请群聊 */
    CHATGROUP,     /* 群聊聊天*/
    
    
};

struct Msg
{
    FUNCTION_OPTION type;
    /* 用户名 */
    char Usrname[20];
    char passwd[20];
    /* 好友名称 */
    char FriName[20];
    /* 群聊名称 */
    char GrpNam[20];
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


inline void cleanScreen()
{
   std::cout << "\033[2J\033[1;1H"; 
}

#endif