#include "gui_components.h"

Status add_button(int winID, int x, int y, int width, int height, char *text, void (*function)()) {
	Window *window = _get_window(winID);

	Button b = (Button){x, y, width, height, function};

	b.text = malloc(strlen(text) + 1);
	memcpy(b.text, text, strlen(text) + 1);

	window->buttonCount++;
	window->buttons = realloc(window->buttons, sizeof(Button) * window->buttonCount);
	window->buttons[window->buttonCount - 1] = b;

	return SUCCESS;
}

Status process_buttons(Window *win) {
	int padding = (comp->windowTitleSize - comp->fontSize) / 2;

	for (int i = 0; i < win->buttonCount; i++) {
		Button *b = &win->buttons[i];
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
			mouse_in_area(win->x + b->x + padding, win->y + comp->windowTitleSize + b->y + padding, b->width,
						  b->height)) {
			if (b->pressed == 0) {
				b->fn();
			}

			b->pressed = 1;
		} else {
			b->pressed = 0;
		}
	}

	return SUCCESS;
}

Status draw_buttons(Window w) {
	int padding = (comp->windowTitleSize - comp->fontSize) / 2;

	for (int j = 0; j < w.buttonCount; j++) {
		Button b = w.buttons[j];

		DrawRectangle(w.x + b.x + padding, w.y + comp->windowTitleSize + b.y + padding, b.width, b.height,
					  b.pressed ? comp->buttonBorderColor : comp->buttonColor);

		DrawRectangleLinesEx(
			(Rectangle){w.x + b.x + padding, w.y + comp->windowTitleSize + b.y + padding, b.width, b.height},
			comp->borderSize / 2, b.pressed ? comp->buttonColor : comp->buttonBorderColor);

		DrawText(w.buttons[j].text, w.x + w.buttons[j].x + 2 * padding,
				 w.y + comp->windowTitleSize + w.buttons[j].y + padding, comp->fontSize, comp->fontColor);
	}

	return SUCCESS;
}