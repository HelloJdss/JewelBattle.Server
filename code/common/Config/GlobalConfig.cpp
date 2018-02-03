//
// Created by LJ on 2018/1/27.
//

#include <common/3lib/lua53/include/lua.hpp>
#include <cstring>
#include "GlobalConfig.h"


IMPLEMENT_SINGLETON(GlobalConfig)

void GlobalConfig::loadDBConfig(char *host, unsigned int *port, char *user, char *password, char *dbname, char *charset) {

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if (luaL_loadfile(L, "db_config.lua") || lua_pcall(L, 0, 0, 0))
	{
		luaL_error(L, "error:%s", lua_tostring(L, -1));
		return;
	}
	lua_getglobal(L, "host");
	lua_getglobal(L, "port");
	lua_getglobal(L, "user");
	lua_getglobal(L, "password");
	lua_getglobal(L, "dbname");
	lua_getglobal(L, "charset");

	strcpy(charset, lua_tostring(L, -1));
	strcpy(dbname, lua_tostring(L, -2));
	strcpy(password, lua_tostring(L, -3));
	strcpy(user, lua_tostring(L, -4));
	*port = (unsigned int)lua_tointeger(L, -5);
	strcpy(host, lua_tostring(L, -6));

}
