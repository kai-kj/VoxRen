#include "scripting.h"

//---- private ---------------------------------------------------------------//

static ScriptStatus _load_script(char *fileName) {
	msg("Initialising script lua state");
	c.sl = luaL_newstate();
	luaL_openlibs(c.sl);

	load_functions(c.sl);

	msg("Loading script file \"%s\"", fileName);

	if (luaL_loadfile(c.sl, fileName) != LUA_OK) {
		err("%s\n", lua_tostring(c.sl, -1));
		return SCRIPTING_FAILURE;
	}

	if (lua_pcall(c.sl, 0, 0, 0) != LUA_OK) {
		err("%s\n", lua_tostring(c.sl, -1));
		return SCRIPTING_FAILURE;
	}

	return SCRIPTING_SUCCESS;
}

static ScriptStatus _run_script(char *functionName) {
	msg("Running script function \"%s\"", functionName);

	lua_getglobal(c.sl, functionName);

	if (lua_pcall(c.sl, 0, 0, 0) != LUA_OK) {
		err("%s\n", lua_tostring(c.sl, -1));
		return SCRIPTING_FAILURE;
	}

	return SCRIPTING_SUCCESS;
}

static void _destroy_script() {
	msg("Closing script lua state");
	lua_close(c.sl);
}

//---- public ----------------------------------------------------------------//

ScriptStatus run_script(char *fileName, char *functionName) {
	if (_load_script(fileName) != SCRIPTING_SUCCESS)
		return SCRIPTING_FAILURE;

	if (_run_script(functionName) != SCRIPTING_SUCCESS)
		return SCRIPTING_FAILURE;

	_destroy_script();

	return SCRIPTING_SUCCESS;
}