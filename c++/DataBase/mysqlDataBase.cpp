#include "mysqlDataBase.h"
stdMysqlDataBase::stdMysqlDataBase()
{
}

stdMysqlDataBase::~stdMysqlDataBase()
{
}
bool stdMysqlDataBase::setConnectDb(const string &dbFileName)
{
    /* 连接数据库 */
    const char *host = "localhost";
    const char *userName = "root";
    const char *passwd = "1";
    /* 3306 为mysql端口号 */
    if (mysql_real_connect(m_sqlDB, host, userName, passwd, dbFileName.c_str(), 3306, NULL, 0) == NULL)
    {
        cout << "file:" << __FILE__ << "line:" << __LINE__ << "connect mysql server error:" << mysql_error(m_sqlDB) << endl;
        return false;
    }
    cout << "连接成功" << endl;
    return true;
}

bool stdMysqlDataBase::excuteSql(const string &sql)
{
     /* 事务的开始 */
    if (mysql_query(m_sqlDB, "START TRANSACTION "))
    {
        cout << "START TRANSACTION" << mysql_error(m_sqlDB) << endl;
        return false;
    }

    if (mysql_query(m_sqlDB, sql.c_str()))
    {
        /* sql语句有问题 */
        cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(m_sqlDB) << endl;
        mysql_query(m_sqlDB, "ROLLBACK");
        return false;
    }
    else
    {
        /* sql语句没有问题 */
        /* 如果返回0，则表示没有行数被影响 */
        if (mysql_affected_rows(m_sqlDB) == 0)
        {
            cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(m_sqlDB) << endl;
            mysql_query(m_sqlDB, "ROLLBACK");
            return false;
        }
    }
    cout << "good sql..." << endl;

    /* 事务的提交 */
    if (mysql_query(m_sqlDB, "COMMIT"))
    {
        cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(m_sqlDB) << endl;
        mysql_query(m_sqlDB, "ROLLBACK");
        return false;
    }
}

vecResult stdMysqlDataBase::querySql(const string &sql)
{
    vector<vector<string>> Datas;

        // 执行查询
    if (mysql_query(m_sqlDB, sql.c_str()) != 0) {
        cout << "mysql_query error: " << mysql_error(m_sqlDB) << endl;
        return Datas;
    }

    // 存储结果集
    MYSQL_RES *result = mysql_store_result(m_sqlDB);
    if (result == NULL) {
        cout << "mysql_store_result error: " << mysql_error(m_sqlDB) << endl;
        return Datas;
    }

    // 获取结果集的列数
    unsigned int cols = mysql_num_fields(result);

    // 获取结果集的行数
    unsigned long long rows = mysql_num_rows(result);

    // 遍历结果集
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        vector<string> rowData;
        for (unsigned int i = 0; i < cols; i++) {
            rowData.push_back(row[i] ? row[i] : "NULL");
        }
        Datas.push_back(rowData);
    }

    // 释放结果集
    mysql_free_result(result);
    return Datas;
}




void stdMysqlDataBase::disConnectDb(const string &dbFileName)
{
    mysql_close(m_sqlDB);
}
