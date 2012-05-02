extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include "lauxlib.h"
}

#pragma once


class Script
{
public:
	Script(void);
	~Script(void);

	bool doFile(char *path);
private:
	lua_State *luaVM;
};

extern Script* gScript;
