#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include "gui.h"

typedef struct GUIState {
	Texture2D renderTexture;

	double prevTime;
	int prevMousePosX;
	int prevMousePosY;

	int firstChar;
	int commandLength;
	char *command;

	int quit;
	int draging;
	int commandMode;

} GUIState;

#endif