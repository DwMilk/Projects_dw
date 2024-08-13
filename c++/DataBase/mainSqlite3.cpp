#include <iostream>
using namespace std;
#include <sqlite3.h>
#include <unistd.h>
#include <iomanip>
#include <vector>
void execute(sqlite3 *m_db)
{
    /* 创建数据库表 */
    const char *sql = "CREATE TABLE IF NOT EXISTS userInfo (      \
                        username text not null,     \
                        passwd text not null        \
                        );";
    /* 执行sql语句 */
    int ret = sqlite3_exec(m_db, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        _exit(-1);
    }
    string userName, passWd;
    cout << "请输入你的用户名" << endl;
    cin >> userName;
    cout << "请输入你的密码" << endl;
    cin >> passWd;

#if 0
    /* 普通插入 */
    char buffer[128] = "INSERT INTO userInfo (username,passwd) VALUES ('%s','%s');";
    char requestSql[128] = {0};
    sprintf(requestSql, buffer, userName.c_str(), passWd.c_str());
    cout << "requestSql:" << requestSql << endl;

    /* 执行sql语句 */
    ret = sqlite3_exec(m_db, requestSql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        _exit(-1);
    }
#endif


#if 0
    /* 插入数据 防注入安全 */
    sql = "INSERT INTO userInfo (username,passwd) values (?,?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        cout << "准备 SQL 语句失败: " << sqlite3_errmsg(m_db) << endl;
        sqlite3_close(m_db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, userName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, passWd.c_str(), -1, SQLITE_STATIC);
    cout << sql << endl;
    /* 执行语句 */
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cout << "插入数据失败: " << sqlite3_errmsg(m_db) << endl;
    }
    else
    {
        cout << "数据插入成功！" << endl;
    }
    sqlite3_exec(m_db, sql, NULL, NULL, NULL);
#endif

    /* todo... */

    /* 修改数据 */
    cout << "请输入你要修改的密码" << endl;
    cin >> passWd;
    char buffer[128] = "UPDATE userInfo SET passwd = '%s' where username = 'lisi';";
    char requestSql[128] = {0};
    sprintf(requestSql, buffer, passWd.c_str());

    /* 执行sql语句 */
    ret = sqlite3_exec(m_db, requestSql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        _exit(-1);
    }
}
int main()
{
    sqlite3 *m_db = nullptr;
    const char *baseName = "./test2.db";
    /* 连接数据库 */
    int ret = sqlite3_open(baseName, &m_db);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3 open error" << sqlite3_errmsg(m_db) << endl;
        _exit(-1);
    }

    const char *sql = "SELECT * FROM userInfo;";
    char **result = NULL;
    int row = 0;
    int col = 0;
    ret = sqlite3_get_table(m_db, sql, &result, &row, &col, NULL);
    if (ret != SQLITE_OK)
    {
        cout << "sqlite3_get_table error" << sqlite3_errmsg(m_db) << endl;
        _exit(-1);
    }

    cout << "row:" << row;
    cout << " col:" << col << endl;

    vector<vector<string>> Datas;

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
    cout << endl;

    cout << "rows: " << Datas.size() << endl;
    for (int idx = 0; idx < Datas.size(); idx++)
    {
        vector<string> rowData = Datas[idx];
        for (int jdx = 0; jdx < rowData.size(); jdx++)
        {
            cout <<  "rowData[" << jdx << "] = " << rowData[jdx] << "\t";
        }
        cout << endl;
    }
    /* 关闭数据库 */
    sqlite3_close(m_db);
    return 0;
}