#ifndef GUI_COMP_H
#define GUI_COMP_H

#include <stdlib.h>
#include <string.h>

#include "k_tools/k_util.h"
#include "raylib/raylib.h"

#include "status.h"
#include "structs.h"

extern GUI *comp;

Status set_comp_address(GUI *ptr);

Status update_gui_components();
Status draw_gui_components();

int add_window(int x, int y, int width, int height, char *title);
Status draw_window(Window w);
Window *_get_window(int id);
Status update_window_pos(Window *win);
int mouse_on_window();
int mouse_in_area(int x, int y, int w, int h);

Status add_textbox(int winID, int x, int y, char *text);
Status change_texbox_text(int winID, int idx, char *text);
Status draw_textboxes(Window w);

Status add_text_input(int winID, int x, int y, int width, int height, char *text, void (*function)(char *));
Status change_text_input_text(int winID, int idx, char *text);
Status procces_text_inputs(Window *win);
Status draw_text_inputs(Window w);

Status add_button(int winID, int x, int y, int width, int height, char *text, void (*function)());
Status process_buttons(Window *win);
Status draw_buttons(Window w);

Status add_colored_box(int winID, int x, int y, int width, int height, Color color);
Status change_colored_box_color(int winID, int idx, Color color);
Status draw_colored_boxes(Window w);

#endif