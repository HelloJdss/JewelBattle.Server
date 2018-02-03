//
// Created by LJ on 2018/1/27.
//
#include "DBConn.h"

#include <cstdio>
#include <cstdarg>
#include <mutex>
#include <mysql.h>

#include "../Config/GlobalConfig.h"
#include <vector>
IMPLEMENT_SINGLETON(DBConn)

static recursive_mutex m;
static MYSQL mysql;
static list<string> sqllist;

bool DBConn::connDB()
{

    char host[128];
    unsigned int port = 0;
    char user[128];
    char password[128];
    char dbname[128];
    char charset[128];
    GlobalConfigMgr->loadDBConfig(host, &port, user, password, dbname, charset);

    printf(host);		    printf("\n");
    printf("%d", port);		printf("\n");
    printf(user);			printf("\n");
    printf(password);		printf("\n");
    printf(dbname);			printf("\n");
    printf(charset);		printf("\n");


    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, host, user, password, dbname, port, nullptr, 0))
    {
        printf("Error connecting to database:%s\n", mysql_error(&mysql));
        return false;
    }
    printf("Connected to DB ok\n");
    return true;
}

void DBConn::execute(const char * sql_fmt, ...)
{
    char sql[1024];
    va_list ap;
    va_start(ap, sql_fmt);
    vsprintf(sql, sql_fmt, ap);
    va_end(ap);
    lock_guard<recursive_mutex> lock(m);
    string sqlstr(sql);
    sqllist.emplace_back(sqlstr);//比push_back更好
}

void DBConn::query(list<map<string, string>> & result, const char * sql_fmt, ...)
{
    char sql[1024];
    va_list ap;
    va_start(ap, sql_fmt);
    vsprintf(sql, sql_fmt, ap);
    va_end(ap);
    if (mysql_query(&mysql, sql) == 0)
    {
        printf("%s    success\n", sql);
    }
    else
    {
        printf("%s    failed: %s\n", sql, mysql_error(&mysql));
        return;
    }
    MYSQL_RES *res; //此结构表示返回行的一个查询结果集
    if (!(res = mysql_store_result(&mysql)))    //获取SQL执行之后返回的结果集
    {
        printf("Couldn't get result from %s\n", mysql_error(&mysql));
        return;
    }
    printf("number of datam line returned: %lld\n", mysql_affected_rows(&mysql));

    vector<string> columnName;//列名
    MYSQL_FIELD *field = nullptr;
    while ((field = mysql_fetch_field(res)) != nullptr)
    {
        string fieldName(field->name);
        columnName.emplace_back(fieldName);
    }
    int columnNum = mysql_num_fields(res); //列数
    MYSQL_ROW row = nullptr;

    while((row = mysql_fetch_row(res)) != nullptr)
    {
        map<string, string> rowResult;
        int count = 0;
        while(count < columnNum)
        {
            rowResult.insert(pair<string, string>(columnName[count], row[count]));
            count++;
        }
        result.emplace_back(rowResult);
    }
}

void DBConn::sqlTimeJob() {
    if (!connDB())
        return;

    //lock_guard<recursive_mutex> lock(m);
    m.lock();
    list<string> sqlcpy(sqllist);
    m.unlock();
    sqllist.clear();
    for (const auto & sqlstr : sqlcpy) {
        printf(sqlstr.c_str());
        if(0 == mysql_query(&mysql, sqlstr.c_str()))
        {
            printf("   ---success\n");
        }
        else
        {
            printf("    failed: %s\n", mysql_error(&mysql));
        }
    }
}
