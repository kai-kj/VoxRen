
#include "gui.h"

//---- private ---------------------------------------------------------------//

SDL_Surface *_get_render_surface() {
	if (s.renderSurface != NULL) {
		SDL_FreeSurface(s.renderSurface);
		s.renderSurface = NULL;
	}

	k_Image *kImage = get_image();

	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
		(void *)kImage->data, kImage->width, kImage->height, 24,
		3 * kImage->width, 0x0000ff, 0x00ff00, 0xff0000, 0x000000);

	if (surface == NULL) {
		msg("%s", SDL_GetError());
		exit(-1);
	}

	k_destroy_image(kImage);

	return surface;
}

void _update_dt() {
	double currentTime = get_time();
	s.dt = currentTime - s.prevTime;
	s.prevTime = currentTime;
}

void print_text(char *string, int x, int y, SDL_Color fg, SDL_Color bg) {
	SDL_Surface *textSurface = TTF_RenderText(s.font, string, fg, bg);

	if (textSurface == NULL)
		exit(-1);

	SDL_Rect textLocation = {x, y, 0, 0};

	SDL_BlitSurface(textSurface, NULL, s.windowSurface, &textLocation);

	SDL_FreeSurface(textSurface);
}

//---- public ----------------------------------------------------------------//

GUIStatus start_main_loop() {
	msg("Starting main loop");

	begin_rendering();

	while (!s.quit) {
		_update_dt();
		procces_events();
		process_kb_input();
		precess_mouse_input();

		if (s.renderSurface != NULL) {
			SDL_FreeSurface(s.renderSurface);
			s.renderSurface = NULL;
		}

		r.readFirstFrame = 1;
		s.renderSurface = _get_render_surface();
		SDL_BlitSurface(s.renderSurface, NULL, s.windowSurface, NULL);

		char fps[100];
		sprintf(fps, "%03d, %03d", (int)(1 / s.dt), (int)(1 / r.dt));
		print_text(fps, 10, 10, (SDL_Color){255, 255, 255},
				   (SDL_Color){0, 0, 0});

		SDL_UpdateWindowSurface(s.window);

		SDL_GetMouseState(&s.prevMousePosX, &s.prevMousePosY);
	}

	end_rendering();

	return GUI_SUCCESS;
}