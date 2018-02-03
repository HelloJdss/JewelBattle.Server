//
// Created by LJ on 2018/1/27.
//

#ifndef JEWELBATTLE_TIMEHELPER_H
#define JEWELBATTLE_TIMEHELPER_H

#include <iostream>
#include <string>
#include <cstdio>
using namespace std;
#ifdef _WIN32
#include <ctime>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <netinet/in.h>
#define SPRINTF sprintf
#endif
time_t getCurrentTimeMillis();
string getCurrentTimeStr();
string getCurrentFileName(string fileFullPath);

#endif //JEWELBATTLE_TIMEHELPER_H
