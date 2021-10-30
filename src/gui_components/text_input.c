#include "gui_components.h"

Status add_text_input(int winID, int x, int y, int width, int height, char *text, void (*function)(char *)) {
	Window *window = _get_window(winID);

	TextInputBox t = (TextInputBox){x, y, width, height, NULL, function, 0};
	t.text = malloc(strlen(text) + 1);
	memcpy(t.text, text, strlen(text) + 1);

	window->textInputBoxCount++;
	window->textInputBoxes = realloc(window->textInputBoxes, sizeof(TextInputBox) * window->textInputBoxCount);
	window->textInputBoxes[window->textInputBoxCount - 1] = t;

	return SUCCESS;
}

Status change_text_input_text(int winID, int idx, char *text) {
	Window *window = _get_window(winID);

	if (window->textInputBoxes[idx].editing == 1) return FAILURE;

	free(window->textInputBoxes[idx].text);
	window->textInputBoxes[idx].text = malloc(strlen(text) + 1);
	memcpy(window->textInputBoxes[idx].text, text, strlen(text) + 1);

	return SUCCESS;
}

Status procces_text_inputs(Window *win) {
	if (comp->state != 1) return FAILURE;

	for (int i = 0; i < win->textInputBoxCount; i++) {
		TextInputBox *t = &win->textInputBoxes[i];
		int onBox = mouse_in_area(win->x + t->x, win->y + comp->windowTitleSize + t->y, t->width, t->height);

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && onBox) {
			change_text_input_text(win->id, i, "");
			t->editing = 1;
			comp->state = 2;
		}

		if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !onBox) || IsKeyPressed(KEY_ENTER)) {
			t->fn(t->text);
			t->editing = 0;
			comp->state = 1;
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

	return SUCCESS;
}

Status draw_text_inputs(Window w) {
	int padding = (comp->windowTitleSize - comp->fontSize) / 2;

	for (int j = 0; j < w.textInputBoxCount; j++) {
		TextInputBox t = w.textInputBoxes[j];

		if (t.editing)
			DrawRectangle(w.x + t.x + padding, w.y + comp->windowTitleSize + t.y + padding, t.width, t.height,
						  comp->buttonColor);

		DrawRectangleLinesEx(
			(Rectangle){w.x + t.x + padding, w.y + comp->windowTitleSize + t.y + padding, t.width, t.height},
			comp->borderSize / 2, comp->buttonBorderColor);

		DrawText(t.text, w.x + t.x + padding + comp->borderSize / 2,
				 w.y + comp->windowTitleSize + t.y + padding + comp->borderSize / 2, comp->fontSize, comp->fontColor);
	}

	return SUCCESS;
}