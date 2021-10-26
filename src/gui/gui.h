#ifndef GUI_H
#define GUI_H

#include "k_tools/k_vectors.h"
#include "raylib/raylib.h"

#include "../renderer/renderer.h"
#include "../scripting/scripting.h"
#include "structs.h"

#define MOV_SPEED 4
#define TURN_SPEED M_PI / 64

typedef char GUIStatus;
#define GUI_SUCCESS 0
#define GUI_FAILURE -1

extern GUIState g;

//--------------------------------------------------------------------------------------------------------------------//
// window                                                                                                             //
//--------------------------------------------------------------------------------------------------------------------//

GUIStatus create_window(int width, int height);
GUIStatus close_window();

//--------------------------------------------------------------------------------------------------------------------//
// main loop                                                                                                          //
//--------------------------------------------------------------------------------------------------------------------//

GUIStatus start_main_loop();

#endif