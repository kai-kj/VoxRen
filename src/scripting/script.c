#include "scripting.h"

//---- private ---------------------------------------------------------------//

static Status _load_script(char *fileName) {
	msg("Initialising script lua state");
	scrp.sl = luaL_newstate();
	luaL_openlibs(scrp.sl);

	load_functions(scrp.sl);

	msg("Loading script file \"%s\"", fileName);

	if (luaL_loadfile(scrp.sl, fileName) != LUA_OK) {
		err("%s\n", lua_tostring(scrp.sl, -1));
		return FAILURE;
	}

	if (lua_pcall(scrp.sl, 0, 0, 0) != LUA_OK) {
		err("%s\n", lua_tostring(scrp.sl, -1));
		return FAILURE;
	}

	return SUCCESS;
}

static Status _run_script(char *functionName) {
	msg("Running script function \"%s\"", functionName);

	lua_getglobal(scrp.sl, functionName);

	if (lua_pcall(scrp.sl, 0, 0, 0) != LUA_OK) {
		err("%s\n", lua_tostring(scrp.sl, -1));
		return FAILURE;
	}

	save_scene("scene");

	return SUCCESS;
}

static void _destroy_script() {
	msg("Closing script lua state");
	lua_close(scrp.sl);
}

//---- public ----------------------------------------------------------------//

Status run_script(char *fileName, char *functionName) {
	if (_load_script(fileName) != SUCCESS) return FAILURE;
	if (_run_script(functionName) != SUCCESS) return FAILURE;
	_destroy_script();

	return SUCCESS;
}