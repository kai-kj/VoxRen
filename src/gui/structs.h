#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include "gui.h"

typedef struct SDLState {
	Uint8 *keyState;
	SDL_Window *window;
	SDL_Surface *windowSurface;
	SDL_Surface *renderSurface;
	TTF_Font *font;

	double dt;
	double prevTime;
	int prevMousePosX;
	int prevMousePosY;

	int firstChar;
	int commandLength;
	char *command;

	int quit;
	int draging;
	int commandMode;

} SDLState;

#endif