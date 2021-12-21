#include "interface.h"

#include "windows/chunks.h"
#include "windows/file.h"
#include "windows/info.h"
#include "windows/material.h"
#include "windows/scene.h"

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

Status initialise_interface() {
	msg("Creating window");

	// raylib init
	SetTraceLogCallback(_ray_log);
	InitWindow(GetScreenWidth(), GetScreenWidth(), "VoxRen");
	SetTargetFPS(30);
	SetExitKey(0);
	ToggleFullscreen();
	HideCursor();

	// init gui state
	Image tmpImg = GenImageColor(ren.image.size.x, ren.image.size.y, BLACK);
	gui.renderTexture = LoadTextureFromImage(tmpImg);
	UnloadImage(tmpImg);

	gui.removeVoxel = 0;
	gui.cameraMoveSpeed = 4;
	gui.cameraLookSpeed = 0.1;
	gui.selectedMaterial = (VoxMaterial){2, (cl_float3){0.5, 0.5, 0.5}, 0, 0, 0};

	// init kGui
	init_gui();

	// create ui components
	create_interface();

	return SUCCESS;
}

Status terminate_interface() {
	msg("Closing window");
	CloseWindow();
	return SUCCESS;
}

Status create_interface() {
	_create_material_window();
	_create_info_window();
	_create_scene_window();
	_create_file_window();
	_create_chunks_window();
	return SUCCESS;
}

Status update_interface() {
	_update_material_window();
	_update_info_window();
	_update_scene_window();
	_update_chunks_window();
	return SUCCESS;
}