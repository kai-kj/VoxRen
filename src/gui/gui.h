#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../renderer/renderer.h"
#include "structs.h"

#define MOV_SPEED 4
#define TURN_SPEED M_PI / 64

typedef char GUIStatus;
#define GUI_SUCCESS 0
#define GUI_FAILURE -1

extern SDLState s;

//---- window ----------------------------------------------------------------//

GUIStatus create_window(int width, int height);
GUIStatus close_window();

//---- main loop -------------------------------------------------------------//

GUIStatus start_main_loop();

//---- input -----------------------------------------------------------------//

GUIStatus procces_events();
GUIStatus process_kb_input();
GUIStatus precess_mouse_input();

//---- place voxel -----------------------------------------------------------//

GUIStatus add_voxel_at_mouse();

#endif