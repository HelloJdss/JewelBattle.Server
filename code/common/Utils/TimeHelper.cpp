//
// Created by LJ on 2018/1/27.
//
#include "TimeHelper.h"

time_t getCurrentTimeMillis()
{
    time_t currentTime = 0;
#ifdef _WIN32
    timeb tb;
	ftime(&tb);
	currentTime = tb.time * 1000 + tb.millitm;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    currentTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
    return currentTime;
}

string getCurrentTimeStr()
{
    struct tm *t;
    char str[64];
    time_t ct = getCurrentTimeMillis() / 1000;
#ifdef _WIN32
    t = localtime(&ct);
#else
    t = *(localtime(&ct));
#endif
    sprintf(str, "%d-%d-%d_%d:%d:%d",
            1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    string r = str;
    return r;
}

string getCurrentFileName(string fileFullPath)
{
    size_t index = 0;
    char c;
#ifdef _WIN32
    c = '\\';
#else
    c = '/';
#endif
    index = fileFullPath.find_last_of(c);
    return fileFullPath.substr(index + 1, fileFullPath.length() - index);
}
