#include "scripting.h"

void _push_material(lua_State *l, VoxMaterial material) {
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
			lua_pushnumber(l, material.v1);
			lua_setfield(l, -2, "brightness");
			break;

		case 2:
			break;

		case 3:
			lua_pushnumber(l, material.v1);
			lua_setfield(l, -2, "tint");
			lua_pushnumber(l, material.v2);
			lua_setfield(l, -2, "fuzz");
			break;

		case 4:
			lua_pushnumber(l, material.v1);
			lua_setfield(l, -2, "tint");
			lua_pushnumber(l, material.v2);
			lua_setfield(l, -2, "fuzz");
			lua_pushnumber(l, material.v3);
			lua_setfield(l, -2, "refIdx");
			break;
	}
}

VoxMaterial _to_material(lua_State *l, int idx) {

	VoxMaterial material;

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
			material.v1 = luaL_checknumber(l, -1);
			lua_pop(l, 1);
			break;

		case 2:
			break;

		case 3:
			lua_getfield(l, idx, "tint");
			material.v1 = luaL_checknumber(l, -1);
			lua_pop(l, 1);
			lua_getfield(l, idx, "fuzz");
			material.v2 = luaL_checknumber(l, -1);
			lua_pop(l, 1);
			break;

		case 4:
			lua_getfield(l, idx, "tint");
			material.v1 = luaL_checknumber(l, -1);
			lua_pop(l, 1);
			lua_getfield(l, idx, "fuzz");
			material.v2 = luaL_checknumber(l, -1);
			lua_pop(l, 1);
			lua_getfield(l, idx, "refIdx");
			material.v3 = luaL_checknumber(l, -1);
			lua_pop(l, 1);
			break;
	}

	return material;
}

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
	VoxMaterial material = _to_material(l, 4);

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

	VoxMaterial material = create_light_source_material(r, g, b, brightness);

	_push_material(l, material);

	return 1;
}

int l_create_lambertian_material(lua_State *l) {
	float r = luaL_checknumber(l, 1);
	float g = luaL_checknumber(l, 2);
	float b = luaL_checknumber(l, 3);

	VoxMaterial material = create_lambertian_material(r, g, b);

	_push_material(l, material);

	return 1;
}

int l_create_metal_material(lua_State *l) {
	float r = luaL_checknumber(l, 1);
	float g = luaL_checknumber(l, 2);
	float b = luaL_checknumber(l, 3);
	float tint = luaL_checknumber(l, 4);
	float fuzz = luaL_checknumber(l, 5);

	VoxMaterial material = create_metal_material(r, g, b, tint, fuzz);

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

	VoxMaterial material = create_dielectric_material(r, g, b, tint, fuzz, refIdx);

	_push_material(l, material);

	return 1;
}

Status load_functions(lua_State *l) {
	lua_pushcfunction(l, l_set_output_properties);
	lua_setglobal(l, "outputProperties");

	lua_pushcfunction(l, l_set_background_properties);
	lua_setglobal(l, "bgProperties");

	lua_pushcfunction(l, l_add_voxel);
	lua_setglobal(l, "addVox");

	lua_pushcfunction(l, l_remove_voxel);
	lua_setglobal(l, "remVox");

	lua_pushcfunction(l, l_set_camera_properties);
	lua_setglobal(l, "cameraProperties");

	lua_pushcfunction(l, l_set_camera_pos);
	lua_setglobal(l, "cameraPos");

	lua_pushcfunction(l, l_create_light_source_material);
	lua_setglobal(l, "lightSource");

	lua_pushcfunction(l, l_create_lambertian_material);
	lua_setglobal(l, "lambertMaterial");

	lua_pushcfunction(l, l_create_metal_material);
	lua_setglobal(l, "metalMaterial");

	lua_pushcfunction(l, l_create_dielectric_material);
	lua_setglobal(l, "glassMaterial");

	return SUCCESS;
}