#ifndef __MYSQLDATABASE_H__
#define __MYSQLDATABASE_H__
#include "stdDataBase.h"
#include <mysql/mysql.h>
using namespace std;
class stdMysqlDataBase : public stdDataBase
{
public:
    /* 构造函数 */
    stdMysqlDataBase();
    /* 析构函数 */
    ~stdMysqlDataBase();
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

    MYSQL * m_sqlDB;    

};












#endif