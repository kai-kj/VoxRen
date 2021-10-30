#include "gui_components.h"

int add_window(int x, int y, int width, int height, char *title) {
	Window w = (Window){comp->windowIDs++, x, y, width, height, title, 0, 0, NULL, 0, NULL, 0, NULL, 0, NULL};

	comp->windowCount++;
	comp->windows = realloc(comp->windows, sizeof(Window) * comp->windowCount);
	comp->windows[comp->windowCount - 1] = w;

	return w.id;
}

Status draw_window(Window w) {
	int padding = (comp->windowTitleSize - comp->fontSize) / 2;

	DrawRectangle(w.x, w.y + comp->windowTitleSize - comp->borderSize, w.width, w.height, comp->windowColor);
	DrawRectangleLinesEx((Rectangle){w.x, w.y + comp->windowTitleSize - comp->borderSize, w.width, w.height},
						 comp->borderSize, comp->borderColor);

	DrawRectangle(w.x, w.y, w.width, comp->windowTitleSize, comp->windowTitleColor);
	DrawRectangleLinesEx((Rectangle){w.x, w.y, w.width, comp->windowTitleSize}, comp->borderSize, comp->borderColor);

	DrawText(w.title, w.x + padding, w.y + padding, comp->fontSize, comp->fontColor);

	return SUCCESS;
}

Window *_get_window(int id) {
	for (int i = 0; i < comp->windowCount; i++)
		if (comp->windows[i].id == id) return &comp->windows[i];

	return NULL;
}

Status update_window_pos(Window *win) {
	if (win->dragging) {
		win->x += comp->mousePosX - comp->prevMousePosX;
		win->y += comp->mousePosY - comp->prevMousePosY;
	}

	int inArea = mouse_in_area(win->x, win->y, win->width, comp->windowTitleSize);

	if (!win->dragging && inArea && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		win->dragging = 1;

	} else if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		win->dragging = 0;
	}

	if (win->dragging ||
		(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouse_in_area(win->x, win->y, win->width, win->height))) {
		Window tmp = comp->windows[comp->windowCount - 1];
		comp->windows[comp->windowCount - 1] = *win;
		*win = tmp;
	}

	return SUCCESS;
}

int mouse_on_window() {
	for (int i = 0; i < comp->windowCount; i++)
		if (mouse_in_area(comp->windows[i].x, comp->windows[i].y, comp->windows[i].width,
						  comp->windows[i].height + comp->windowTitleSize))
			return 1;

	return 0;
}

int mouse_in_area(int x, int y, int w, int h) {
	return comp->mousePosX > x && comp->mousePosX < x + w && comp->mousePosY > y && comp->mousePosY < y + h;
}