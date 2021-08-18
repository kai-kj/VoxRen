#include "scripting.h"

//---- private ---------------------------------------------------------------//

static ScriptStatus _load_script(char *fileName) {
	msg("Initialising lua state");
	c.l = luaL_newstate();
	luaL_openlibs(c.l);

	const struct luaL_Reg luaFuncs[] = {
		{"set_output_properties", l_set_output_properties},
		{"set_background_properties", l_set_background_properties},
		{"add_voxel", l_add_voxel},
		{"remove_voxel", l_remove_voxel},
		{"set_camera_properties", l_set_camera_properties},
		{"set_camera_pos", l_set_camera_pos},
		{"create_light_source_material", l_create_light_source_material},
		{"create_lambertian_material", l_create_lambertian_material},
		{"create_metal_material", l_create_metal_material},
		{"create_dielectric_material", l_create_dielectric_material},
		{NULL, NULL}};

	lua_newtable(c.l);
	luaL_setfuncs(c.l, luaFuncs, 0);
	lua_setglobal(c.l, "r");

	msg("Loading script file %s", fileName);

	if (luaL_loadfile(c.l, fileName) != LUA_OK) {
		err("%s\n", lua_tostring(c.l, -1));
		return SCRIPTING_FAILURE;
	}

	if (lua_pcall(c.l, 0, 0, 0) != LUA_OK) {
		err("%s\n", lua_tostring(c.l, -1));
		return SCRIPTING_FAILURE;
	}

	return SCRIPTING_SUCCESS;
}

static ScriptStatus _run_script(char *functionName) {
	msg("Running script function %s", functionName);

	lua_getglobal(c.l, functionName);

	if (lua_pcall(c.l, 0, 0, 0) != LUA_OK) {
		err("%s\n", lua_tostring(c.l, -1));
		return SCRIPTING_FAILURE;
	}

	return SCRIPTING_SUCCESS;
}

static void _destroy_script() {
	msg("Closing lua state");
	lua_close(c.l);
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