//
// Created by LJ on 2018/1/27.
//

#ifndef JEWELBATTLE_GLOBALCONFIG_H
#define JEWELBATTLE_GLOBALCONFIG_H

#include "../Utils/Singleton.h"

class GlobalConfig {
    DECLARE_SINGLETON(GlobalConfig)

public:
    static void loadDBConfig(char *host, unsigned int *port, char *user, char *password, char *dbname, char *charset);
};

#define GlobalConfigMgr GlobalConfig::GetInstance()
#endif //JEWELBATTLE_GLOBALCONFIG_H
