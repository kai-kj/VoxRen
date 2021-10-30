#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <lua5.4/lauxlib.h>
#include <lua5.4/lua.h>
#include <lua5.4/lualib.h>

#include "../renderer/renderer.h"
#include "status.h"
#include "structs.h"

extern ScriptState scrp;

//--------------------------------------------------------------------------------------------------------------------//
// command wrapper                                                                                                    //
//--------------------------------------------------------------------------------------------------------------------//

int l_set_output_properties(lua_State *l);
int l_set_background_properties(lua_State *l);
int l_add_voxel(lua_State *l);
int l_remove_voxel(lua_State *l);
int l_set_camera_properties(lua_State *l);
int l_set_camera_pos(lua_State *l);
int l_create_light_source_material(lua_State *l);
int l_create_lambertian_material(lua_State *l);
int l_create_metal_material(lua_State *l);
int l_create_dielectric_material(lua_State *l);
Status load_functions(lua_State *l);

//--------------------------------------------------------------------------------------------------------------------//
// script                                                                                                             //
//--------------------------------------------------------------------------------------------------------------------//

Status run_script(char *fileName, char *functionName);

//--------------------------------------------------------------------------------------------------------------------//
// commands                                                                                                           //
//--------------------------------------------------------------------------------------------------------------------//

Status init_command_interpreter();
Status close_command_interpreter();
Status run_command(char *command);

#endif