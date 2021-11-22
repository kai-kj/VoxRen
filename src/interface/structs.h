#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include "interface.h"

typedef struct GUIState {
	Texture2D renderTexture;

	int width;
	int height;

	double prevTime;

	int renderMousePosX;
	int renderMousePosY;

	int quit;

	int windowIDs;
	int windowCount;
	Window *windows;

	int infoWindow;
	int materialWindow;
	int fileWindow;
	int cameraWindow;

	VoxMaterial selectedMaterial;
	cl_float3 selectedBgColor;

	char *fileName;

	int scriptCount;
	char **scripts;

	float cameraMoveSpeed;
	float cameraLookSpeed;

} GUIState;

#endif