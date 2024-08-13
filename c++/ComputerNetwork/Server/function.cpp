#include "function.h"
#include <string>
#include <unistd.h>
#include <cstring>
using namespace std;
Function::Function(const StdTcpSocketPtr &clientInfo)
{
    m_clientInfo = clientInfo;
    // m_sqliteDB.setConnectDb("./test2.db");
    /* mysql数据库 */
    m_mysqlDB.setConnectDb("demoTest");
}

Function::~Function()
{
}

void Function::handleRegisterInfo(const Msg &msg)
{
    std::cout << "username:" << msg.Usrname << std::endl;
    std::cout << "passwd:" << msg.passwd << std::endl;

#if 0
    ReplyMsg responseMsg;
    responseMsg.type = REGISTER;
    /* 判断用户是否存在 */
    if (userIsExist(msg.Usrname) == true)
    {
        responseMsg.statue_code = REGISTER_USEREXIST;
    }
    else
    {
        responseMsg.statue_code = REGISTER_SUCCESS;
        /* todo... */
    }
    /* 将消息发送会客户端 */
    this->m_clientInfo->sendMessage(static_cast<const void *>(&responseMsg), sizeof(responseMsg));
#endif
    ReplyMsg responseMsg;
    responseMsg.type = REGISTER;
    /* 判断用户是否存在 */
    if (userIsExist(msg.Usrname) == true)
    {
        cout << "用户已存在,数据库判断 " << endl;
        responseMsg.statue_code = REGISTER_USEREXIST;
    }
    else
    {
        responseMsg.statue_code = REGISTER_SUCCESS;
        /* todo... */
    }
    /* 将消息发送会客户端 */
    this->m_clientInfo->sendMessage(static_cast<const void *>(&responseMsg), sizeof(responseMsg));
}

void Function::handleLoginInfo(const Msg &msg)
{
    std::cout << "username:" << msg.Usrname << std::endl;
    std::cout << "passwd:" << msg.passwd << std::endl;

    string username(msg.Usrname);
    string passwd(msg.passwd);
    ReplyMsg responseMsg;
    memset(&responseMsg, 0, sizeof(responseMsg));

    responseMsg.type = LOGIN;
    if (username == "zhangsan")
    {
        if (passwd == "123456")
        {
            /* 用户名和密码匹配 - 登陆成功 */
            responseMsg.statue_code = LOGIN_SUCCESS;
            sleep(1);
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
    m_clientInfo->sendMessage(static_cast<const void *>(&responseMsg), sizeof(responseMsg));
}

void Function::handleAddFrienfInfo(const Msg &msg)
{
    cout << "msg.FriendName:" << msg.FriName << endl;
}

void Function::handleDeleteFrienfInfo(const Msg &msg)
{
    cout << "msg.Deletefriend:" << msg.FriName << endl;
}

void Function::handleFriendChatInfo(const Msg &msg)
{
    cout << "msg.FriendChat:" << msg.FriName << endl;
}

void Function::handleFriendListInfo(const Msg &msg)
{
    cout << "msg.FriendList" << endl;
}

void Function::handleNewGroupInfo(const Msg &msg)
{
    cout << "msg.NewGroup" << msg.GrpName << endl;
}

void Function::handleExitGroupInfo(const Msg &msg)
{
    cout << "msg.ExitGroup" << msg.GrpName << endl;
}

void Function::handleJoinGroupInfo(const Msg &msg)
{
    cout << "msg.JoinGroup" << msg.GrpName << endl;
}

void Function::handleInviteInfo(const Msg &msg)
{
    cout << "msg.InviteGroup" << msg.GrpName << endl;
}

void Function::handleGroupChatInfo(const Msg &msg)
{
    cout << "msg.CurChatGroup" << msg.GrpName << endl;
}

bool Function::userIsExist(const string & username)
{
#if 0

    string sql = "select count(1) from teacher where name = '%s';";
    char requestSql[128] = {0};
    sprintf(requestSql,sql.c_str(),username);
    cout << requestSql << endl;
    vector<vector<string>> res = m_mysqlDB.querySql(requestSql);
    if(res[0][0] == "0")
    {
        return false;
    }
    return true;
#endif
    const char *sql = "SELECT count(1) from teacher where name = ?";
    MYSQL_STMT* stmt = mysql_stmt_init(m_mysqlDB.getConnection());
    if(!stmt)
    {
        cerr << "mysql_stmt_init() failed" << endl;
        return false;
    }
    if(mysql_stmt_prepare(stmt,sql,strlen(sql)) != 0)
    {
        cerr << "mysql_stmt_prepare() failed:" <<mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return false;
    }

    MYSQL_BIND bind[1];
    memset(bind,0,sizeof(bind));

    unsigned long length = username.length();
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void * )username.c_str();
    bind[0].buffer_length = username.length();
    bind[0].length = &length;
    
    if(mysql_stmt_bind_param(stmt,bind) != 0)
    {
        cerr << "mysql_stmt_bind_param() failed:" << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return false;
    }

    if(mysql_stmt_execute(stmt) != 0)
    {
        cerr << "mysql_stmt_execute() failed:" << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return false;
    }

    int count = 0;
    MYSQL_BIND result[1];
    memset(result,0,sizeof(result));
    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &count;

    if(mysql_stmt_bind_result(stmt,result) != 0)
    {
        cerr << "mysql_stmt_bind_result() failed: " << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return false;
    }
    if(mysql_stmt_fetch(stmt) != 0)
    {
        cerr << "mysql_stmt_fetch() failed:" << mysql_stmt_error(stmt) << endl;
        mysql_stmt_close(stmt);
        return false;
    }


    mysql_stmt_close(stmt);
    return count > 0;
}

bool Function::userIsMatch(const char *username, const char *passwd)
{
    return true;
}

bool Function::userIsOnline(const char *username)
{
    return true;
}
