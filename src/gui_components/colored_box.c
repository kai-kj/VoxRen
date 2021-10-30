#include "gui_components.h"

Status add_colored_box(int winID, int x, int y, int width, int height, Color color) {
	Window *window = _get_window(winID);

	ColoredBox b = (ColoredBox){x, y, width, height, color};

	window->coloredBoxCount++;
	window->coloredBoxes = realloc(window->coloredBoxes, sizeof(ColoredBox) * window->coloredBoxCount);
	window->coloredBoxes[window->coloredBoxCount - 1] = b;

	return SUCCESS;
}

Status change_colored_box_color(int winID, int idx, Color color) {
	Window *window = _get_window(winID);
	window->coloredBoxes[idx].color = color;

	return SUCCESS;
}

Status draw_colored_boxes(Window w) {
	int padding = (comp->windowTitleSize - comp->fontSize) / 2;

	for (int j = 0; j < w.coloredBoxCount; j++) {
		ColoredBox b = w.coloredBoxes[j];

		DrawRectangle(w.x + b.x + padding, w.y + comp->windowTitleSize + b.y + padding, b.width, b.height, b.color);
	}

	return SUCCESS;
}