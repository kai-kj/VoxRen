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

GUIStatus create_window(int width, int height) {
	g.width = width;
	g.height = height;

	msg("Creating window");

	SetTraceLogCallback(_ray_log);
	InitWindow(g.width, g.height, "VoxRen");
	SetTargetFPS(30);

	Image tmpImg = GenImageColor(g.width, g.height, BLACK);
	g.renderTexture = LoadTextureFromImage(tmpImg);
	UnloadImage(tmpImg);

	g.commandLength = 1;
	g.command = '\0';

	g.draging = 0;
	g.commandMode = 0;

	return GUI_SUCCESS;
}

GUIStatus close_window() {
	msg("Closing window");
	CloseWindow();
	return GUI_SUCCESS;
}