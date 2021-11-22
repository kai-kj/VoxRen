#include "kGui.h"

Component create_label(char *text) {
	Component component;
	component.type = 1;
	component.data.label.text = malloc(strlen(text) + 1);
	memcpy(component.data.label.text, text, strlen(text) + 1);
	return component;
}

void _draw_label(Label *label, int x, int y) {
	DrawText(label->text, x, y, kGS.settings.fontSize, kGS.settings.fontColor);
}