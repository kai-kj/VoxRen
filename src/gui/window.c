#include "gui.h"

void _ray_log(int msgType, const char *text, va_list args) {
#ifndef K_UTIL_DEBUG
	if (msgType != LOG_INFO && msgType != LOG_DEBUG) {
#endif

		printf("[\e[1m\e[34mRAY\e[0m] ");

		switch (msgType) {
			case LOG_INFO:
				printf("[\e[1m\e[33mDBG\e[0m] ");
				break;
			case LOG_ERROR:
				printf("[\e[1m\e[31mERR\e[0m] ");
				break;
			case LOG_WARNING:
				printf("[\e[1m\e[32mMSG\e[0m] ");
				break;
			case LOG_DEBUG:
				printf("[\e[1m\e[33mDBG\e[0m] ");
				break;
			default:
				break;
		}

		vprintf(text, args);
		printf("\n");

#ifndef K_UTIL_DEBUG
	}
#endif
}

GUIStatus create_window() {
	msg("Creating window");

	SetTraceLogCallback(_ray_log);
	InitWindow(GetScreenWidth(), GetScreenWidth(), "VoxRen");
	SetTargetFPS(30);

	if (!IsWindowFullscreen()) ToggleFullscreen();

	Image tmpImg = GenImageColor(r.image.size.x, r.image.size.x, BLACK);
	g.renderTexture = LoadTextureFromImage(tmpImg);
	UnloadImage(tmpImg);

	g.commandLength = 1;
	g.command = '\0';
	g.windowIDs = 0;
	g.windowCount = 0;
	g.windows = NULL;
	g.state = 1;

	g.settings.fontSize = 20;
	g.settings.fontColor = BLACK;
	g.settings.windowTitleSize = 30;
	g.settings.windowColor = LIGHTGRAY;
	g.settings.windowTitleColor = GRAY;
	g.settings.borderSize = 4;
	g.settings.borderColor = BLACK;
	g.settings.buttonColor = GRAY;
	g.settings.buttonBorderColor = DARKGRAY;

	g.selectedMaterial = (VoxMaterial){2, (cl_float3){0.5, 0.5, 0.5}, 0, 0, 0};

	create_ui();

	return GUI_SUCCESS;
}

GUIStatus close_window() {
	msg("Closing window");
	CloseWindow();
	return GUI_SUCCESS;
}