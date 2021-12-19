#include "interface.h"

void _draw_cursor() {
	DrawCircle(GetMouseX(), GetMouseY(), 5, BLACK);
	DrawCircle(GetMouseX(), GetMouseY(), 2, WHITE);
}

Status draw_additional_interface() {
	_draw_cursor();
	return SUCCESS;
}