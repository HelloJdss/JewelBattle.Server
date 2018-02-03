//
// Created by LJ on 2018/1/27.
//

#ifndef JEWELBATTLE_DEBUG_H
#define JEWELBATTLE_DEBUG_H

#include "../Utils/Singleton.h"
#define LOG_INFO  0
#define LOG_WARN  1
#define LOG_ERROR 2

const char LOG_LEVEL [][6] = {"INFO", "WARN", "ERROR"};
class Debug {
    DECLARE_SINGLETON(Debug)
public:
    static void doLog(int log_level,
                      const char *fileName,
                      const char *funcName,
                      int line,
                      const char *fmt, ...);
    static void doPrint(const char * fmt, ...);
};


#define	DEBUG_INFO(fmt, ...)	Debug::GetInstance()->doLog(LOG_INFO, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define	DEBUG_WARN(fmt, ...)	Debug::GetInstance()->doLog(LOG_WARN, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define	DEBUG_ERROR(fmt, ...)	Debug::GetInstance()->doLog(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define DEBUG_PRINT(fmt, ...)   Debug::GetInstance()->doPrint(fmt, ##__VA_ARGS__)

#endif //JEWELBATTLE_DEBUG_H
