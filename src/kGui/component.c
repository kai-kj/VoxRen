#include "kGui.h"

void _process_all_components() {
	for (int i = 0; i < kGS.windowCount; i++) {
		_process_component(kGS.windows[i].grid, kGS.windows[i].x + kGS.settings.borderSize,
						   kGS.windows[i].y + kGS.settings.borderSize + kGS.settings.titleBarHeight);
	}
}

ComponentID add_component(ComponentID destId, int x, int y, Component component) {
	Component dest = kGS.components[destId];
	if (dest.type != 0) return -1;

	kGS.componentCount++;
	kGS.components = realloc(kGS.components, sizeof(Component) * kGS.componentCount);
	kGS.components[kGS.componentCount - 1] = component;

	dest.data.grid.components[x + y * dest.data.grid.cols] = kGS.componentCount - 1;

	_process_all_components();

	return kGS.componentCount - 1;
}

void change_component_text(ComponentID compId, char *format, ...) {
	char *dest;

	switch (kGS.components[compId].type) {
		case 1:
			dest = kGS.components[compId].data.label.text;
			break;
		case 2:
			dest = kGS.components[compId].data.button.text;
			break;
		case 3:
			dest = kGS.components[compId].data.textbox.text;
			break;
		default:
			return;
	}

	char buff[MAX_TEXT_LENGTH];
	va_list args;
	va_start(args, format);
	vsnprintf(buff, MAX_TEXT_LENGTH, format, args);
	va_end(args);

	dest = realloc(dest, strlen(buff) + 1);
	memcpy(dest, buff, strlen(buff) + 1);
}

Component *_get_component(ComponentID compId) {
	return &kGS.components[compId];
}

void _draw_component(ComponentID compID, int x, int y) {
	if (compID < -1) return;
	Component *component = _get_component(compID);

	switch (component->type) {
		case 0:
			_draw_grid(&component->data.grid, x, y);
			break;
		case 1:
			_draw_label(&component->data.label, x, y);
			break;
		case 2:
			_draw_button(&component->data.button, x, y);
			break;
		case 3:
			_draw_textbox(&component->data.textbox, x, y);
			break;
		case 5:
			_draw_custom_component(&component->data.customComponent, x, y);
			break;
	}
}

void _process_component(ComponentID compID, int x, int y) {
	if (compID < 0) return;
	Component *component = _get_component(compID);

	switch (component->type) {
		case 0:
			_process_grid(&component->data.grid, x, y);
			break;
		case 2:
			_process_button(&component->data.button, x, y);
			break;
		case 3:
			_process_textbox(&component->data.textbox, x, y);
			break;
		case 4:
			_process_pos_tracker(&component->data.posTracker, x, y);
			break;
	}
}

int is_editing() {
	for (int i = 0; i < kGS.componentCount; i++)
		if (kGS.components[i].type == 3 && kGS.components[i].data.textbox.editing) return 1;
	return 0;
}