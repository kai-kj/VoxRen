
#include "gui.h"

//---- private ---------------------------------------------------------------//

SDL_Surface *_get_render_surface() {
	if (g.renderSurface != NULL) {
		SDL_FreeSurface(g.renderSurface);
		g.renderSurface = NULL;
	}

	k_Image *kImage = get_image();

	SDL_Surface *surface = SDL_CreateSurfaceFrom(
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
	g.dt = currentTime - g.prevTime;
	g.prevTime = currentTime;
}

void _update_size() {
	int width, height;
	SDL_GetWindowSize(g.window, &width, &height);

	if (r.image.size.x != width || r.image.size.y != height) {
		r.restartRender = 1;
		r.image.size.x = width;
		r.image.size.y = height;
		set_output_properties(width, height);
	}
}

void _print_text(char *string, int x, int y, SDL_Color fg) {
	SDL_Surface *textSurface = TTF_RenderText_Solid(g.font, string, fg);

	if (textSurface == NULL)
		exit(-1);

	SDL_Rect textLocation = {x, y, 0, 0};

	SDL_BlitSurface(textSurface, NULL, g.windowSurface, &textLocation);

	SDL_FreeSurface(textSurface);
}

//---- public ----------------------------------------------------------------//

GUIStatus start_main_loop() {
	msg("Starting main loop");

	begin_rendering();

	while (!g.quit) {
		_update_dt();
		// _update_size();
		procces_events();
		process_kb_input();
		precess_mouse_input();

		if (g.renderSurface != NULL) {
			SDL_FreeSurface(g.renderSurface);
			g.renderSurface = NULL;
		}

		r.readFirstFrame = 1;
		g.renderSurface = _get_render_surface();
		SDL_BlitSurface(g.renderSurface, NULL, g.windowSurface, NULL);

		char fps[100];
		sprintf(fps, "%03d, %03d", (int)(1 / g.dt), (int)(1 / r.dt));
		_print_text(fps, 10, 10, (SDL_Color){255, 255, 255});

		int width, height;
		SDL_GL_GetDrawableSize(g.window, &width, &height);

		char command[100];

		if (g.commandLength != 1)
			sprintf(command, ">> %s", g.command);
		else
			sprintf(command, ">>");

		if (g.commandMode) {
			_print_text(command, 10, height - 10 - 24,
						(SDL_Color){255, 255, 255});
		}

		SDL_UpdateWindowSurface(g.window);

		SDL_GetMouseState(&g.prevMousePosX, &g.prevMousePosY);
	}

	end_rendering();

	return GUI_SUCCESS;
}