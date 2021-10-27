#ifndef GUI_H
#define GUI_H

#include <string.h>

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

GUIStatus create_window();
GUIStatus close_window();

//--------------------------------------------------------------------------------------------------------------------//
// main loop                                                                                                          //
//--------------------------------------------------------------------------------------------------------------------//

GUIStatus start_main_loop();

//--------------------------------------------------------------------------------------------------------------------//
// interface                                                                                                          //
//--------------------------------------------------------------------------------------------------------------------//

GUIStatus draw_aim();
GUIStatus update_info_window();
GUIStatus update_material_window();
GUIStatus create_ui();

//--------------------------------------------------------------------------------------------------------------------//
// components //
//--------------------------------------------------------------------------------------------------------------------//

int add_window(int x, int y, int width, int height, char *title);
GUIStatus add_textbox(int winID, int x, int y, char *text);
GUIStatus change_texbox_text(int winID, int idx, char *text);
GUIStatus add_button(int winID, int x, int y, int width, int height, char *text, void *(function)());
GUIStatus update_windows();
GUIStatus draw_windows();

#endif