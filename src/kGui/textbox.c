#include "kGui.h"

#include "kGui.h"

Component create_textbox(int width, int height, char *text, void (*fn)(char *)) {
	Component component;
	component.type = 3;
	component.data.textbox.onlyDigits = 0;
	component.data.textbox.editing = 0;
	component.data.textbox.mouseInBounds = 0;
	component.data.textbox.width = width;
	component.data.textbox.height = height;
	component.data.textbox.fn = fn;
	component.data.textbox.text = malloc(strlen(text) + 1);
	memcpy(component.data.textbox.text, text, strlen(text) + 1);

	return component;
}

Component create_numbox(int width, int height, char *text, void (*fn)(char *)) {
	Component component = create_textbox(width, height, text, fn);
	component.data.textbox.onlyDigits = 1;
	return component;
}

void _draw_textbox(Textbox *textbox, int x, int y) {
	Rectangle btnRect = (Rectangle){x, y, textbox->width, textbox->height};

	if (textbox->editing)
		_draw_rectangle(btnRect, kGS.settings.textboxPressedColor);
	else
		_draw_rectangle(btnRect, kGS.settings.textboxReleasedColor);

	DrawRectangleLinesEx(btnRect, kGS.settings.textboxBorderSize, kGS.settings.borderColor);

	DrawText(textbox->text, x + kGS.settings.padding, y + kGS.settings.padding, kGS.settings.fontSize,
			 kGS.settings.fontColor);
}

// only gets called when window is under mouse
void _process_textbox(Textbox *textbox, int x, int y) {
	int mouseOnTextbox = _mouse_in_area(x, y, textbox->width, textbox->height);

	if (mouseOnTextbox && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) textbox->editing = 1;
	if (mouseOnTextbox) textbox->mouseInBounds = 1;
}

// always gets called
void _manage_textbox(Textbox *textbox) {
	int clickedOutside = IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !textbox->mouseInBounds;
	if (textbox->editing && (clickedOutside || IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_ESCAPE))) {
		textbox->editing = 0;
		if (textbox->fn != NULL) textbox->fn(textbox->text);
	}

	if (textbox->editing) {
		int key = GetCharPressed();

		while (key > 0) {
			if ((textbox->onlyDigits && ((key >= 48 && key <= 57) || key == 46)) ||
				(!textbox->onlyDigits && (key >= 32 && key <= 126))) {
				int length = strlen(textbox->text);
				textbox->text = realloc(textbox->text, length + 2);
				textbox->text[length] = (char)key;
				textbox->text[length + 1] = '\0';
			}

			key = GetCharPressed();
		}

		if (IsKeyPressed(KEY_BACKSPACE)) {
			int length = strlen(textbox->text);

			if (length > 0) {
				textbox->text = realloc(textbox->text, length);
				textbox->text[length - 1] = '\0';
			}
		}
	}

	textbox->mouseInBounds = 0;
}

void _manage_textboxes() {
	for (int i = 0; i < kGS.componentCount; i++) {
		if (kGS.components[i].type == 3) {
			_manage_textbox(&kGS.components[i].data.textbox);
		}
	}
}