#include "scripting.h"

ScriptStatus init_command_interpreter() {
	msg("Initialising script lua state");
	c.cl = luaL_newstate();
	luaL_openlibs(c.cl);

	load_functions(c.cl);

	return SCRIPTING_SUCCESS;
}

ScriptStatus close_command_interpreter() {
	msg("Closing command lua state");
	lua_close(c.cl);
}

ScriptStatus run_command(char *command) {
	msg("Running command \"%s\"", command);

	if (luaL_dostring(c.cl, command) != LUA_OK) {
		err("%s\n", lua_tostring(c.cl, -1));
		return SCRIPTING_FAILURE;
	}

	return SCRIPTING_SUCCESS;
}