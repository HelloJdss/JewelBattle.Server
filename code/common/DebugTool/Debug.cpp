//
// Created by LJ on 2018/1/27.
//


#include <cstdio>
#include <cstdarg>

#include "Debug.h"

IMPLEMENT_SINGLETON(Debug)

void Debug::doLog(int log_level,
                  const char *fileName,
                  const char *funcName,
                  int line,
                  const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char msg[512];
    vsprintf(msg, fmt, ap);
    va_end(ap);
    //TODO: save log msg to file
    printf(LOG_LEVEL[log_level]);printf("\n");
    printf(fileName);printf("\n");
    printf(funcName);printf("\n");
    printf("%d", line);printf("\n");
    printf(msg);printf("\n");
}

void Debug::doPrint(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}