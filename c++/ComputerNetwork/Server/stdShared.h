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

#endif