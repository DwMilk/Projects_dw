#ifndef __STDDATABASE_H__
#define __STDDATABASE_H__
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using vecResult = vector<vector<string>>;

class stdDataBase
{
public:

    /* 虚析构 */
    virtual ~stdDataBase() = default;

public:
    /* 连接数据库 */
    virtual bool setConnectDb(const string & dbFileName) = 0;

    /* 执行sql语句 */
    virtual bool excuteSql(const string & sql) = 0;

    /* 查询sql语句*/
    virtual vector<vector<string>> querySql(const string &sql) = 0;

    /* 关闭数据库 */
    virtual void disConnectDb(const string & dbFileName) = 0;

private:
    vecResult Datas;
};
















#endif