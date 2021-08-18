#include "gui.h"

//---- private ---------------------------------------------------------------//

//---- public ----------------------------------------------------------------//

GUIStatus create_window(int width, int height) {
	// window
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		err("Failed to initialise SDL");
		return GUI_FAILURE;
	}

	s.window = NULL;
	s.windowSurface = NULL;
	s.renderSurface = NULL;

	s.commandLength = 1;
	s.command = '\0';

	s.draging = 0;
	s.commandMode = 0;

	msg("Creating window");

	s.window = SDL_CreateWindow("pathtracer", SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED, width, height,
								SDL_WINDOW_SHOWN);

	if (s.window == NULL) {
		err("Failed to crate window");
		return GUI_FAILURE;
	}

	s.windowSurface = SDL_GetWindowSurface(s.window);

	s.keyState = (Uint8 *)SDL_GetKeyboardState(NULL);

	// font
	if (TTF_Init() < 0) {
		err("Failed to initialise SDL_TTF");
		return GUI_FAILURE;
	}

	s.font = TTF_OpenFont("data/assets/opensans.ttf", 24);

	if (s.font == NULL) {
		err("Failed to load font");
		return GUI_FAILURE;
	}

	return GUI_SUCCESS;
}

GUIStatus close_window() {
	msg("Closing window");

	if (s.renderSurface != NULL) {
		SDL_FreeSurface(s.renderSurface);
		s.renderSurface = NULL;
	}

	if (s.renderSurface != NULL) {
		SDL_FreeSurface(s.windowSurface);
		s.windowSurface = NULL;
	}

	SDL_DestroyWindow(s.window);
	TTF_Quit();
	SDL_Quit();

	return GUI_SUCCESS;
}