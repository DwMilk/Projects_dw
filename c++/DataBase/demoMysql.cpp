#include <iostream>
using namespace std;
#include <mysql/mysql.h>
#include <unistd.h>
#include <iomanip>
int main()
{
    MYSQL *sqlDB = nullptr;
    sqlDB = mysql_init(NULL);
    if (sqlDB == nullptr)
    {
        cout << "mysql_init failed." << endl;
        exit(-1);
    }

    cout << "初始化成功..." << endl;

    /* 连接数据库 */
    const char *host = "localhost";
    const char *userName = "root";
    const char *passwd = "1";
    const char *baseName = "demoTest";
    /* 3306 为mysql端口号 */
    if (mysql_real_connect(sqlDB, host, userName, passwd, baseName, 3306, NULL, 0) == NULL)
    {
        cout << "file:" << __FILE__ << "line:" << __LINE__ << "connect mysql server error:" << mysql_error(sqlDB) << endl;
        exit(-1);
    }
    cout << "连接成功" << endl;

    /* 事务的开始 */
    if (mysql_query(sqlDB, "START TRANSACTION "))
    {
        cout << "START TRANSACTION" << mysql_error(sqlDB) << endl;
        exit(-1);
    }
    /* 执行sql语句 */
    const char *sql1 = "update students set age = age - 100 where name = 'zhangsan';";
    const char *sql2 = "update students set age = age + 100 where name = 'lisi';";

    if (mysql_query(sqlDB, sql1))
    {
        /* sql语句有问题 */
        cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(sqlDB) << endl;
        mysql_query(sqlDB, "ROLLBACK");
        exit(-1);
    }
    else
    {
        /* sql语句没有问题 */
        /* 如果返回0，则表示没有行数被影响 */
        if (mysql_affected_rows(sqlDB) == 0)
        {
            cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(sqlDB) << endl;
            mysql_query(sqlDB, "ROLLBACK");
            exit(-1);
        }
    }
    cout << "good sql1..." << endl;
    if (mysql_query(sqlDB, sql2))
    {
        cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(sqlDB) << endl;
        mysql_query(sqlDB, "ROLLBACK");
        exit(-1);
    }
    else
    {
        /* 如果返回0，则表示没有行数被影响 */
        if (mysql_affected_rows(sqlDB) == 0)
        {
            cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(sqlDB) << endl;
            mysql_query(sqlDB, "ROLLBACK");
            exit(-1);
        }
    }
    cout << "good sql2..." << endl;

    /* 事务的提交 */
    if (mysql_query(sqlDB, "COMMIT"))
    {
        cout << "file:" << __FILE__ << "line:" << __LINE__ << mysql_error(sqlDB) << endl;
        mysql_query(sqlDB, "ROLLBACK");
        exit(-1);
    }
    /* 创建数据库表 */
    const char *sql = "CREATE TABLE if not exists students(               \
                        id INT auto_increment primary key,   \
                        name text not null,                  \
                        age int,                             \
                        grade varchar(10)                    \
                        );";
    int ret = mysql_query(sqlDB, sql);
    if (ret != 0)
    {
        cout << "mysql_query error:" << mysql_error(sqlDB) << endl;
        exit(-1);
    }
    cout << "创建数据表成功" << endl;

    // /* 插入数据 */
    // sql = "insert into students (name,age,grade) values ('zhangsan',18,'三年级');";
    // ret = mysql_query(sqlDB, sql);
    // if (ret != 0)
    // {
    //     cout << "mysql_query error:" << mysql_error(sqlDB) << endl;
    //     exit(-1);
    // }
    // sql = "insert into students (name,age,grade) values ('lisi',20,'一年级');";
    // ret = mysql_query(sqlDB, sql);
    // if (ret != 0)
    // {
    //     cout << "mysql_query error:" << mysql_error(sqlDB) << endl;
    //     exit(-1);
    // }
    // sql = "insert into students (name,age,grade) values ('zhangsan',18,'五年级');";
    // ret = mysql_query(sqlDB, sql);
    // if (ret != 0)
    // {
    //     cout << "mysql_query error:" << mysql_error(sqlDB) << endl;
    //     exit(-1);
    // }

    // /* 修改数据 */
    // sql = "update students set age = 666 where name = 'zhangsan';";
    // ret = mysql_query(sqlDB, sql);
    // if (ret != 0)
    // {
    //     cout << "mysql_query error:" << mysql_error(sqlDB) << endl;
    //     exit(-1);
    // }

    /* 查询语句 */
    sql = "select * from students;";
    ret = mysql_query(sqlDB, sql);
    if (ret != 0)
    {
        cout << "mysql_query error:" << mysql_error(sqlDB) << endl;
        exit(-1);
    }
    cout << "查询成功" << endl;

    /* 结果集合 */
    MYSQL_RES *result = mysql_store_result(sqlDB);
    if (result == NULL)
    {
        cout << "mysql_stroe_result error:" << mysql_error(sqlDB) << endl;
        exit(-1);
    }

    /* 获取结果集的列数 */
    unsigned int cols = mysql_num_fields(result);
    cout << "列数：" << cols << endl;

    /* 获取结果集的行数 */
    unsigned long long rows = mysql_num_rows(result);
    cout << "行数：" << rows << endl;

    /* 获取数据库表的字段 */
    MYSQL_FIELD *field = NULL;
    while ((field = mysql_fetch_field(result)) != NULL)
    {
        cout << left << setw(12) << field->name;
    }
    cout << endl;

    /* 获取数据 */
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        /* 每个row变量表示一行 */
        for (int col = 0; col < cols; col++)
        {
            cout << left << setw(12) << row[col];
        }
        cout << endl;
    }

    /* 释放结果集 */
    mysql_free_result(result);

    /* 关闭数据库 */
    mysql_close(sqlDB);
    return 0;
}
