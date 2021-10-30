#include "scripting.h"

Status init_command_interpreter() {
	msg("Initialising script lua state");
	scrp.cl = luaL_newstate();
	luaL_openlibs(scrp.cl);
	load_functions(scrp.cl);

	return SUCCESS;
}

Status close_command_interpreter() {
	msg("Closing command lua state");
	lua_close(scrp.cl);

	return SUCCESS;
}

Status run_command(char *command) {
	msg("Running command \"%s\"", command);

	if (luaL_dostring(scrp.cl, command) != LUA_OK) {
		err("%s\n", lua_tostring(scrp.cl, -1));
		return FAILURE;
	}

	ren.restartRender = 1;

	return SUCCESS;
}