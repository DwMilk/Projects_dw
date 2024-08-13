#include "stdDataBase.h"
#include "sqliteDataBase.h"

sqliteDataBase::sqliteDataBase() : m_db(nullptr)
{
}

sqliteDataBase::~sqliteDataBase()
{
}

bool sqliteDataBase::setConnectDb(const string &dbFileName)
{
    /* 连接数据库 */
    int ret = sqlite3_open(dbFileName.c_str(), &m_db);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        return false;
    }
    return true;
}

bool sqliteDataBase::excuteSql(const string &sql)
{
    /* 执行sql语句 */
    int ret = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        return false;
    }
    return true;
}

vecResult sqliteDataBase::querySql(const string &sql)
{
    vector<vector<string>> Datas;

    char **result = NULL;
    int row = 0;
    int col = 0;
    int ret = sqlite3_get_table(m_db, sql.c_str(), &result, &row, &col, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3_get_table error" << sqlite3_errmsg(m_db) << endl;
        return Datas;
    }

    for (int i = 1; i <= row; i++)
    {
        vector<string> rowData;
        for (int j = 0; j < col; j++)
        {
            // cout << left << setw(16) << result[i * col + j] << " ";
            rowData.push_back(result[i * col + j]);
        }
        Datas.push_back(rowData);
    }
    return Datas;
}

void sqliteDataBase::disConnectDb(const string &dbFileName)
{
    sqlite3_close(m_db);
}
