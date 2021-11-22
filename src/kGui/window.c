#include "kGui.h"

int _mouse_on_window(Window *w) {
	Grid grid = (*_get_component(w->grid)).data.grid;
	int width = grid.cellWidth * grid.cols;
	int height = grid.cellHeight * grid.rows;
	if (_mouse_in_area(w->x, w->y, width, height + kGS.settings.titleBarHeight))
		return 1;
	else
		return 0;
}

int mouse_on_windows() {
	for (int i = 0; i < kGS.windowCount; i++)
		if (_mouse_on_window(&kGS.windows[i])) return 1;
	return 0;
}

Window *_window_under_mouse() {
	Window *w = NULL;

	for (int i = 0; i < kGS.windowCount; i++) {
		if (_mouse_on_window(&kGS.windows[i])) w = &kGS.windows[i];
	}

	return w;
}

void _move_window_to_front(Window *w) {
	int idx = 0;
	for (int i = 0; i < kGS.windowCount; i++)
		if (w == &kGS.windows[i]) idx = i;

	Window tmp = *w;
	for (int i = idx; i < kGS.windowCount - 1; i++)
		kGS.windows[i] = kGS.windows[i + 1];

	kGS.windows[kGS.windowCount - 1] = tmp;
}

ComponentID create_window(int x, int y, int cols, int rows, int cellWidth, int cellHeight, char *title) {
	Component grid = create_grid(cols, rows, cellWidth, cellHeight);

	kGS.componentCount++;
	kGS.components = realloc(kGS.components, sizeof(Component) * kGS.componentCount);
	kGS.components[kGS.componentCount - 1] = grid;

	Window w;
	w.x = x;
	w.y = y;
	w.title = malloc(strlen(title) + 1);
	w.dragging = 0;
	w.grid = kGS.componentCount - 1;
	memcpy(w.title, title, strlen(title) + 1);

	kGS.windowCount++;
	kGS.windows = realloc(kGS.windows, sizeof(Window) * kGS.windowCount);
	kGS.windows[kGS.windowCount - 1] = w;

	return kGS.componentCount - 1;
}

void _draw_window(Window *w) {
	Grid grid = (*_get_component(w->grid)).data.grid;
	int width = grid.cellWidth * grid.cols + kGS.settings.borderSize * 2;
	int height = grid.cellHeight * grid.rows + kGS.settings.borderSize * 2;

	// title bar
	Rectangle titleRect = (Rectangle){w->x, w->y, width, kGS.settings.titleBarHeight};
	_draw_rectangle(titleRect, kGS.settings.titleBarColor);
	DrawRectangleLinesEx(titleRect, kGS.settings.borderSize, kGS.settings.borderColor);
	DrawText(w->title, w->x + kGS.settings.borderSize * 2,
			 w->y + kGS.settings.titleBarHeight / 2 - kGS.settings.fontSize / 2, kGS.settings.fontSize,
			 kGS.settings.fontColor);

	// main body
	Rectangle mainRect = (Rectangle){w->x, w->y + kGS.settings.titleBarHeight - kGS.settings.borderSize, width, height};
	_draw_rectangle(mainRect, kGS.settings.windowColor);
	DrawRectangleLinesEx(mainRect, kGS.settings.borderSize, kGS.settings.borderColor);

	_draw_component(w->grid, w->x + kGS.settings.borderSize,
					w->y + kGS.settings.borderSize + kGS.settings.titleBarHeight);
}

void _process_window(Window *w) {
	Grid grid = (*_get_component(w->grid)).data.grid;
	int width = grid.cellWidth * grid.cols;
	int height = grid.cellHeight * grid.rows;

	int mouseOnTitleBar = _mouse_in_area(w->x, w->y, width, kGS.settings.titleBarHeight);
	int mouseOnWindow = _mouse_in_area(w->x, w->y, width, height + kGS.settings.titleBarHeight);

	if (w->dragging) {
		w->x += GetMouseX() - kGS.prevMouseX;
		w->y += GetMouseY() - kGS.prevMouseY;
	}

	int clickable = _window_under_mouse() == w;

	// window dragging
	if (clickable && mouseOnTitleBar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) w->dragging = 1;
	if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) w->dragging = 0;

	// bring window to front
	if (w->dragging || (mouseOnWindow && clickable && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
		_move_window_to_front(w);
	}

	if (clickable)
		_process_component(w->grid, w->x + kGS.settings.borderSize,
						   w->y + kGS.settings.borderSize + kGS.settings.titleBarHeight);
}
