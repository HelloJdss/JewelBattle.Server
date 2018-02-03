//
// Created by LJ on 2018/1/27.
//

#ifndef JEWELBATTLE_DBCONN_H
#define JEWELBATTLE_DBCONN_H

#include <iostream>
#include <list>
#include <string>
#include <list>
#include <map>
#include "../Utils/Singleton.h"
using namespace std;
class DBConn {
    DECLARE_SINGLETON(DBConn)

public:
    static bool connDB();

public:
    static void execute(const char *sql_fmt, ...);
    static void query(list<map<string, string>> & result, const char *sql_fmt, ...);
    static void sqlTimeJob();
};

#define DB DBConn::GetInstance()
#endif //JEWELBATTLE_DBCONN_H
