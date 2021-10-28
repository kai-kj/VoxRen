#include "gui.h"

int _mouse_in_area(int x, int y, int w, int h) {
	return g.mousePosX > x && g.mousePosX < x + w && g.mousePosY > y && g.mousePosY < y + h;
}

void _draw_window(Window w) {
	int padding = (g.settings.windowTitleSize - g.settings.fontSize) / 2;

	DrawRectangle(w.x, w.y + g.settings.windowTitleSize - g.settings.borderSize, w.width, w.height,
				  g.settings.windowColor);
	DrawRectangleLinesEx((Rectangle){w.x, w.y + g.settings.windowTitleSize - g.settings.borderSize, w.width, w.height},
						 g.settings.borderSize, g.settings.borderColor);

	DrawRectangle(w.x, w.y, w.width, g.settings.windowTitleSize, g.settings.windowTitleColor);
	DrawRectangleLinesEx((Rectangle){w.x, w.y, w.width, g.settings.windowTitleSize}, g.settings.borderSize,
						 g.settings.borderColor);

	DrawText(w.title, w.x + padding, w.y + padding, g.settings.fontSize, g.settings.fontColor);

	for (int j = 0; j < w.coloredBoxCount; j++) {
		ColoredBox b = w.coloredBoxes[j];

		DrawRectangle(w.x + b.x + padding, w.y + g.settings.windowTitleSize + b.y + padding, b.width, b.height,
					  b.color);
	}

	for (int j = 0; j < w.buttonCount; j++) {
		Button b = w.buttons[j];

		DrawRectangle(w.x + b.x + padding, w.y + g.settings.windowTitleSize + b.y + padding, b.width, b.height,
					  b.pressed ? g.settings.buttonBorderColor : g.settings.buttonColor);

		DrawRectangleLinesEx(
			(Rectangle){w.x + b.x + padding, w.y + g.settings.windowTitleSize + b.y + padding, b.width, b.height},
			g.settings.borderSize / 2, b.pressed ? g.settings.buttonColor : g.settings.buttonBorderColor);

		DrawText(w.buttons[j].text, w.x + w.buttons[j].x + 2 * padding,
				 w.y + g.settings.windowTitleSize + w.buttons[j].y + padding, g.settings.fontSize,
				 g.settings.fontColor);
	}

	for (int j = 0; j < w.textBoxCount; j++) {
		TextBox t = w.textBoxes[j];

		DrawText(t.text, w.x + t.x + padding, w.y + g.settings.windowTitleSize + t.y + padding, g.settings.fontSize,
				 g.settings.fontColor);
	}

	for (int j = 0; j < w.textInputBoxCount; j++) {
		TextInputBox t = w.textInputBoxes[j];

		if (t.editing)
			DrawRectangle(w.x + t.x + padding, w.y + g.settings.windowTitleSize + t.y + padding, t.width, t.height,
						  g.settings.buttonColor);

		DrawRectangleLinesEx(
			(Rectangle){w.x + t.x + padding, w.y + g.settings.windowTitleSize + t.y + padding, t.width, t.height},
			g.settings.borderSize / 2, g.settings.buttonBorderColor);

		DrawText(t.text, w.x + t.x + padding + g.settings.borderSize / 2,
				 w.y + g.settings.windowTitleSize + t.y + padding + g.settings.borderSize / 2, g.settings.fontSize,
				 g.settings.fontColor);
	}
}

Window *_get_window(int id) {
	for (int i = 0; i < g.windowCount; i++)
		if (g.windows[i].id == id) return &g.windows[i];

	return NULL;
}

void _update_window_pos(Window *win) {
	if (win->dragging) {
		win->x += g.mousePosX - g.prevMousePosX;
		win->y += g.mousePosY - g.prevMousePosY;
	}

	int inArea = _mouse_in_area(win->x, win->y, win->width, g.settings.windowTitleSize);

	if (!win->dragging && inArea && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		win->dragging = 1;

	} else if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		win->dragging = 0;
	}

	if (win->dragging ||
		(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && _mouse_in_area(win->x, win->y, win->width, win->height))) {
		Window tmp = g.windows[g.windowCount - 1];
		g.windows[g.windowCount - 1] = *win;
		*win = tmp;
	}
}

void _process_buttons(Window *win) {
	int padding = (g.settings.windowTitleSize - g.settings.fontSize) / 2;

	for (int i = 0; i < win->buttonCount; i++) {
		Button *b = &win->buttons[i];
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
			_mouse_in_area(win->x + b->x + padding, win->y + g.settings.windowTitleSize + b->y + padding, b->width,
						   b->height)) {
			if (b->pressed == 0) {
				b->fn();
			}

			b->pressed = 1;
		} else {
			b->pressed = 0;
		}
	}
}

void _procces_text_input_boxes(Window *win) {
	if (g.state != 1) return;

	for (int i = 0; i < win->textInputBoxCount; i++) {
		TextInputBox *t = &win->textInputBoxes[i];
		int onBox = _mouse_in_area(win->x + t->x, win->y + g.settings.windowTitleSize + t->y, t->width, t->height);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && onBox) {
			change_text_input_box_text(win->id, i, "");
			t->editing = 1;
			g.state = 2;
		}

		if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !onBox) || IsKeyPressed(KEY_ENTER)) {
			t->fn(t->text);
			t->editing = 0;
			g.state = 1;
		}

		if (t->editing) {
			int key = GetCharPressed();

			while (key > 0) {
				if ((key >= 48 && key <= 57) || key == 46) {
					int length = strlen(t->text);
					t->text = realloc(t->text, length + 2);
					t->text[length] = (char)key;
					t->text[length + 1] = '\0';
				}

				key = GetCharPressed();
			}

			if (IsKeyPressed(KEY_BACKSPACE)) {
				int length = strlen(t->text);

				if (length > 0) {
					t->text = realloc(t->text, length);
					t->text[length - 1] = '\0';
				}
			}
		}
	}
}

int mouse_on_window() {
	for (int i = 0; i < g.windowCount; i++)
		if (_mouse_in_area(g.windows[i].x, g.windows[i].y, g.windows[i].width,
						   g.windows[i].height + g.settings.windowTitleSize))
			return 1;

	return 0;
}

GUIStatus add_textbox(int winID, int x, int y, char *text) {
	Window *window = _get_window(winID);

	TextBox t = (TextBox){x, y};
	t.text = malloc(strlen(text) + 1);
	memcpy(t.text, text, strlen(text) + 1);

	window->textBoxCount++;
	window->textBoxes = realloc(window->textBoxes, sizeof(TextBox) * window->textBoxCount);
	window->textBoxes[window->textBoxCount - 1] = t;

	return GUI_SUCCESS;
}

GUIStatus change_texbox_text(int winID, int idx, char *text) {
	Window *window = _get_window(winID);

	free(window->textBoxes[idx].text);
	window->textBoxes[idx].text = malloc(strlen(text) + 1);
	memcpy(window->textBoxes[idx].text, text, strlen(text) + 1);

	return GUI_SUCCESS;
}

GUIStatus add_button(int winID, int x, int y, int width, int height, char *text, void (*function)()) {
	Window *window = _get_window(winID);

	Button b = (Button){x, y, width, height, function};

	b.text = malloc(strlen(text) + 1);
	memcpy(b.text, text, strlen(text) + 1);

	window->buttonCount++;
	window->buttons = realloc(window->buttons, sizeof(Button) * window->buttonCount);
	window->buttons[window->buttonCount - 1] = b;

	return GUI_SUCCESS;
}

GUIStatus add_colored_box(int winID, int x, int y, int width, int height, Color color) {
	Window *window = _get_window(winID);

	ColoredBox b = (ColoredBox){x, y, width, height, color};

	window->coloredBoxCount++;
	window->coloredBoxes = realloc(window->coloredBoxes, sizeof(ColoredBox) * window->coloredBoxCount);
	window->coloredBoxes[window->coloredBoxCount - 1] = b;

	return GUI_SUCCESS;
}

GUIStatus change_colored_box_color(int winID, int idx, Color color) {
	Window *window = _get_window(winID);
	window->coloredBoxes[idx].color = color;

	return GUI_SUCCESS;
}

GUIStatus add_text_input_box(int winID, int x, int y, int width, int height, char *text, void (*function)(char *)) {
	Window *window = _get_window(winID);

	TextInputBox t = (TextInputBox){x, y, width, height, NULL, function, 0};
	t.text = malloc(strlen(text) + 1);
	memcpy(t.text, text, strlen(text) + 1);

	window->textInputBoxCount++;
	window->textInputBoxes = realloc(window->textInputBoxes, sizeof(TextInputBox) * window->textInputBoxCount);
	window->textInputBoxes[window->textInputBoxCount - 1] = t;

	return GUI_SUCCESS;
}

GUIStatus change_text_input_box_text(int winID, int idx, char *text) {
	Window *window = _get_window(winID);

	if (window->textInputBoxes[idx].editing == 1) return GUI_FAILURE;

	free(window->textInputBoxes[idx].text);
	window->textInputBoxes[idx].text = malloc(strlen(text) + 1);
	memcpy(window->textInputBoxes[idx].text, text, strlen(text) + 1);

	return GUI_SUCCESS;
}

GUIStatus update_windows() {
	for (int i = 0; i < g.windowCount; i++) {
		_update_window_pos(&g.windows[i]);
		_process_buttons(&g.windows[i]);
		_procces_text_input_boxes(&g.windows[i]);
	}

	return GUI_SUCCESS;
}

GUIStatus draw_windows() {
	for (int i = 0; i < g.windowCount; i++) {
		Window w = g.windows[i];
		_draw_window(w);
	}

	return GUI_SUCCESS;
}

int add_window(int x, int y, int width, int height, char *title) {
	Window w = (Window){g.windowIDs++, x, y, width, height, title, 0, 0, NULL, 0, NULL, 0, NULL, 0, NULL};

	g.windowCount++;
	g.windows = realloc(g.windows, sizeof(Window) * g.windowCount);
	g.windows[g.windowCount - 1] = w;

	return w.id;
}