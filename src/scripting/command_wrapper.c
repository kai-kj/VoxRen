#include "scripting.h"

//---- private ---------------------------------------------------------------//

void _push_material(lua_State *l, Material material) {
	lua_newtable(l);

	lua_pushinteger(l, material.type);
	lua_setfield(l, -2, "type");

	lua_newtable(l);
	lua_pushnumber(l, material.color.x);
	lua_setfield(l, -2, "r");
	lua_pushnumber(l, material.color.y);
	lua_setfield(l, -2, "g");
	lua_pushnumber(l, material.color.z);
	lua_setfield(l, -2, "b");
	lua_setfield(l, -2, "color");

	switch (material.type) {
	case 1:
		lua_pushnumber(l, material.details.lightSource.brightness);
		lua_setfield(l, -2, "brightness");
		break;

	case 2:
		break;

	case 3:
		lua_pushnumber(l, material.details.metal.tint);
		lua_setfield(l, -2, "tint");
		lua_pushnumber(l, material.details.metal.fuzz);
		lua_setfield(l, -2, "fuzz");
		break;

	case 4:
		lua_pushnumber(l, material.details.dielectric.tint);
		lua_setfield(l, -2, "tint");
		lua_pushnumber(l, material.details.dielectric.fuzz);
		lua_setfield(l, -2, "fuzz");
		lua_pushnumber(l, material.details.dielectric.refIdx);
		lua_setfield(l, -2, "refIdx");
		break;
	}
}

Material _to_material(lua_State *l, int idx) {

	Material material;

	lua_getfield(l, idx, "type");
	material.type = luaL_checkinteger(l, -1);
	lua_pop(l, 1);

	lua_getfield(l, idx, "color");
	lua_getfield(l, -1, "r");
	material.color.x = luaL_checknumber(l, -1);
	lua_pop(l, 1);
	lua_getfield(l, -1, "g");
	material.color.y = luaL_checknumber(l, -1);
	lua_pop(l, 1);
	lua_getfield(l, -1, "b");
	material.color.z = luaL_checknumber(l, -1);
	lua_pop(l, 1);
	lua_pop(l, 1);

	switch (material.type) {
	case 1:
		lua_getfield(l, idx, "brightness");
		material.details.lightSource.brightness = luaL_checknumber(l, -1);
		lua_pop(l, 1);
		break;

	case 2:
		break;

	case 3:
		lua_getfield(l, idx, "tint");
		material.details.metal.tint = luaL_checknumber(l, -1);
		lua_pop(l, 1);
		lua_getfield(l, idx, "fuzz");
		material.details.metal.fuzz = luaL_checknumber(l, -1);
		lua_pop(l, 1);
		break;

	case 4:
		lua_getfield(l, idx, "tint");
		material.details.dielectric.tint = luaL_checknumber(l, -1);
		lua_pop(l, 1);
		lua_getfield(l, idx, "fuzz");
		material.details.dielectric.fuzz = luaL_checknumber(l, -1);
		lua_pop(l, 1);
		lua_getfield(l, idx, "refIdx");
		material.details.dielectric.refIdx = luaL_checknumber(l, -1);
		lua_pop(l, 1);
		break;
	}

	return material;
}

//---- public ----------------------------------------------------------------//

int l_set_output_properties(lua_State *l) {
	int width = luaL_checkinteger(l, 1);
	int height = luaL_checkinteger(l, 2);

	set_output_properties(width, height);

	return 0;
}

int l_set_background_properties(lua_State *l) {
	float r = luaL_checknumber(l, 1);
	float g = luaL_checknumber(l, 2);
	float b = luaL_checknumber(l, 3);
	float brightness = luaL_checknumber(l, 4);

	set_background_properties(r, g, b, brightness);

	return 0;
}

int l_add_voxel(lua_State *l) {
	float x = luaL_checknumber(l, 1);
	float y = luaL_checknumber(l, 2);
	float z = luaL_checknumber(l, 3);
	Material material = _to_material(l, 4);

	add_voxel(x, y, z, material);

	return 0;
}

int l_remove_voxel(lua_State *l) {
	float x = luaL_checknumber(l, 1);
	float y = luaL_checknumber(l, 2);
	float z = luaL_checknumber(l, 3);

	remove_voxel(x, y, z);

	return 0;
}

int l_set_camera_properties(lua_State *l) {
	float sensorWidth = luaL_checknumber(l, 1);
	float focalLength = luaL_checknumber(l, 2);
	float aperture = luaL_checknumber(l, 3);
	float exposure = luaL_checknumber(l, 4);

	set_camera_properties(sensorWidth, focalLength, aperture, exposure);

	return 0;
}

int l_set_camera_pos(lua_State *l) {
	float x = luaL_checknumber(l, 1);
	float y = luaL_checknumber(l, 2);
	float z = luaL_checknumber(l, 3);
	float rx = luaL_checknumber(l, 4);
	float ry = luaL_checknumber(l, 5);

	set_camera_pos(x, y, z, rx, ry);

	return 0;
}

int l_create_light_source_material(lua_State *l) {
	float r = luaL_checknumber(l, 1);
	float g = luaL_checknumber(l, 2);
	float b = luaL_checknumber(l, 3);
	float brightness = luaL_checknumber(l, 4);

	Material material = create_light_source_material(r, g, b, brightness);

	_push_material(l, material);

	return 1;
}
int l_create_lambertian_material(lua_State *l) {
	float r = luaL_checknumber(l, 1);
	float g = luaL_checknumber(l, 2);
	float b = luaL_checknumber(l, 3);

	Material material = create_lambertian_material(r, g, b);

	_push_material(l, material);

	return 1;
}
int l_create_metal_material(lua_State *l) {
	float r = luaL_checknumber(l, 1);
	float g = luaL_checknumber(l, 2);
	float b = luaL_checknumber(l, 3);
	float tint = luaL_checknumber(l, 4);
	float fuzz = luaL_checknumber(l, 5);

	Material material = create_metal_material(r, g, b, tint, fuzz);

	_push_material(l, material);

	return 1;
}
int l_create_dielectric_material(lua_State *l) {
	float r = luaL_checknumber(l, 1);
	float g = luaL_checknumber(l, 2);
	float b = luaL_checknumber(l, 3);
	float tint = luaL_checknumber(l, 4);
	float fuzz = luaL_checknumber(l, 5);
	float refIdx = luaL_checknumber(l, 6);

	Material material = create_dielectric_material(r, g, b, tint, fuzz, refIdx);

	_push_material(l, material);

	return 1;
}

ScriptStatus load_functions(lua_State *l) {
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

	lua_newtable(l);
	luaL_setfuncs(l, luaFuncs, 0);
	lua_setglobal(l, "r");

	return SCRIPTING_SUCCESS;
}