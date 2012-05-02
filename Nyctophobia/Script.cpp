#include "Script.h"
#include "d3dUtil.h"
#include <string>


Script::Script(void)
{
	luaVM = lua_open(0);

	if (NULL == luaVM)
	{
		MessageBox(0, "Error Initializing LUA",0,0);
		return;
	}
	/*luaopen_base(luaVM);
	luaopen_io(luaVM);
	luaopen_os(luaVM);
	luaopen_string(luaVM);
	luaopen_math(luaVM);
	luaopen_table(luaVM);
	luaopen_debug(luaVM);*/

}


Script::~Script(void)
{
	lua_close(luaVM);
	//delete luaVM;
}

bool Script::doFile(char* path)
{
	std::string s = "Data\\Scripts\\" + std::string(path);
	//luaL_loadfile(luaVM, s.c_str());
	return true;
}