#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include "gui.h"

typedef struct GUIState {
	Texture2D renderTexture;

	int width;
	int height;

	GUI comp;

	double prevTime;

	int renderMousePosX;
	int renderMousePosY;

	int firstChar;
	int commandLength;
	char *command;

	int quit;
	int state;

	int windowIDs;
	int windowCount;
	Window *windows;

	int infoWindow;
	int materialWindow;

	VoxMaterial selectedMaterial;

} GUIState;

#endif