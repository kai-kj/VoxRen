#ifndef SCRIPTING_STRUCTS_H
#define SCRIPTING_STRUCTS_H

#include "scripting.h"

typedef struct ScriptState {
	lua_State *sl; // for scripts
	lua_State *cl; // for commands

} ScriptState;

#endif