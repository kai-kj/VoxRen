#include "gui_components.h"

Status add_textbox(int winID, int x, int y, char *text) {
	Window *window = _get_window(winID);

	TextBox t = (TextBox){x, y};
	t.text = malloc(strlen(text) + 1);
	memcpy(t.text, text, strlen(text) + 1);

	window->textBoxCount++;
	window->textBoxes = realloc(window->textBoxes, sizeof(TextBox) * window->textBoxCount);
	window->textBoxes[window->textBoxCount - 1] = t;

	return SUCCESS;
}

Status change_texbox_text(int winID, int idx, char *text) {
	Window *window = _get_window(winID);

	free(window->textBoxes[idx].text);
	window->textBoxes[idx].text = malloc(strlen(text) + 1);
	memcpy(window->textBoxes[idx].text, text, strlen(text) + 1);

	return SUCCESS;
}

Status draw_textboxes(Window w) {
	int padding = (comp->windowTitleSize - comp->fontSize) / 2;

	for (int j = 0; j < w.textBoxCount; j++) {
		TextBox t = w.textBoxes[j];

		DrawText(t.text, w.x + t.x + padding, w.y + comp->windowTitleSize + t.y + padding, comp->fontSize,
				 comp->fontColor);
	}

	return SUCCESS;
}