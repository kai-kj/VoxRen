#include "gui.h"

//---- private ---------------------------------------------------------------//

//---- public ----------------------------------------------------------------//

GUIStatus create_window(int width, int height) {
	// window
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		err("Failed to initialise SDL");
		return GUI_FAILURE;
	}

	g.window = NULL;
	g.windowSurface = NULL;
	g.renderSurface = NULL;

	g.commandLength = 1;
	g.command = '\0';

	g.draging = 0;
	g.commandMode = 0;

	msg("Creating window");

	g.window = SDL_CreateWindow("pathtracer", SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED, width, height,
								SDL_WINDOW_SHOWN);

	if (g.window == NULL) {
		err("Failed to crate window");
		return GUI_FAILURE;
	}

	// SDL_SetWindowResizable(g.window, SDL_TRUE);

	g.windowSurface = SDL_GetWindowSurface(g.window);

	g.keyState = (Uint8 *)SDL_GetKeyboardState(NULL);

	// font
	if (TTF_Init() < 0) {
		err("Failed to initialise SDL_TTF");
		return GUI_FAILURE;
	}

	g.font = TTF_OpenFont("data/assets/opensans.ttf", 24);

	if (g.font == NULL) {
		err("Failed to load font");
		return GUI_FAILURE;
	}

	return GUI_SUCCESS;
}

GUIStatus close_window() {
	msg("Closing window");

	if (g.renderSurface != NULL) {
		SDL_FreeSurface(g.renderSurface);
		g.renderSurface = NULL;
	}

	if (g.renderSurface != NULL) {
		SDL_FreeSurface(g.windowSurface);
		g.windowSurface = NULL;
	}

	SDL_DestroyWindow(g.window);
	TTF_Quit();
	SDL_Quit();

	return GUI_SUCCESS;
}