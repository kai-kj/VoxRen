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

Status create_interface() {
	msg("Creating window");

	// raylib init
	SetTraceLogCallback(_ray_log);
	InitWindow(GetScreenWidth(), GetScreenWidth(), "VoxRen");
	SetTargetFPS(30);
	SetExitKey(0);
	ToggleFullscreen();

	// init kGui
	init_gui();

	// init gui state
	Image tmpImg = GenImageColor(ren.image.size.x, ren.image.size.x, BLACK);
	gui.renderTexture = LoadTextureFromImage(tmpImg);
	UnloadImage(tmpImg);

	gui.windowIDs = 0;
	gui.windowCount = 0;
	gui.windows = NULL;
	gui.cameraMoveSpeed = 4;
	gui.cameraLookSpeed = 0.1;
	gui.selectedMaterial = (VoxMaterial){2, (cl_float3){0.5, 0.5, 0.5}, 0, 0, 0};

	// create ui components
	// create_ui();

	return SUCCESS;
}

Status destroy_interface() {
	msg("Closing window");
	CloseWindow();
	return SUCCESS;
}