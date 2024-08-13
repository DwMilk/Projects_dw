#ifndef __SQLITEDATABASE_H__
#define __SQLITEDATABASE_H__
#include "stdDataBase.h"
#include <sqlite3.h>
#include <unistd.h>
class sqliteDataBase : public stdDataBase
{
public:
    /* 无参构造 */
    sqliteDataBase();

    // /* 有参构造*/
    // sqliteDataBase();

    /* 析构函数 */
    ~sqliteDataBase();
public:
    /* 连接数据库 */
    bool setConnectDb(const string & dbFileName);

    /* 执行sql语句 */
    bool excuteSql(const string & sql);

    /* 查询sql语句 */
    vecResult querySql(const string & sql);

    /* 关闭数据库 */
    void disConnectDb(const string & dbFileName);

private:

    sqlite3 * m_db;
};









#endif