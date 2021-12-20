#include "interface.h"

#define CURSOR_SIZE 8
#define CURSOR_BORDER_SIZE 2

void _draw_cursor() {
	DrawRectangle(GetMouseX() - CURSOR_SIZE / 2, GetMouseY() - CURSOR_SIZE / 2, CURSOR_SIZE, CURSOR_SIZE, BLACK);
	DrawRectangle(GetMouseX() - CURSOR_SIZE / 2 + CURSOR_BORDER_SIZE,
				  GetMouseY() - CURSOR_SIZE / 2 + CURSOR_BORDER_SIZE, CURSOR_SIZE - CURSOR_BORDER_SIZE * 2,
				  CURSOR_SIZE - CURSOR_BORDER_SIZE * 2, gui.removeVoxel ? RED : GREEN);
}

Status draw_additional_interface() {
	_draw_cursor();
	return SUCCESS;
}