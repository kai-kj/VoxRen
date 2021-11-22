#include "kGui.h"

void init_gui() {
	kGS.windowCount = 0;
	kGS.componentCount = 0;

	kGS.prevMouseX = GetMouseX();
	kGS.prevMouseY = GetMouseY();

	kGS.settings.padding = 4;

	kGS.settings.fontColor = BLACK;
	kGS.settings.fontSize = 20;

	kGS.settings.windowColor = RAYWHITE;

	kGS.settings.titleBarHeight = 30;
	kGS.settings.titleBarColor = LIGHTGRAY;

	kGS.settings.borderSize = 4;
	kGS.settings.borderColor = DARKGRAY;

	kGS.settings.buttonBorderSize = 2;
	kGS.settings.buttonReleasedColor = LIGHTGRAY;
	kGS.settings.buttonPressedColor = GRAY;

	kGS.settings.textboxBorderSize = 2;
	kGS.settings.textboxReleasedColor = RAYWHITE;
	kGS.settings.textboxPressedColor = LIGHTGRAY;
}

void draw_gui() {
	for (int i = 0; i < kGS.windowCount; i++) {
		_draw_window(&kGS.windows[i]);
	}
}

void process_gui() {
	for (int i = 0; i < kGS.windowCount; i++) {
		_process_window(&kGS.windows[i]);
	}

	_manage_textboxes();

	kGS.prevMouseX = GetMouseX();
	kGS.prevMouseY = GetMouseY();
}