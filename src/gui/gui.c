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
		msg("%s\n", SDL_GetError());
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

void _procces_events() {
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			s.quit = 1;
		}
	}
}

void _process_kb_input() {
	float movementSpeed = 1;
	float turnSpeed = M_PI / 32;

	SDL_PumpEvents();

	if (s.keyState[SDL_SCANCODE_Q]) {
		s.quit = 1;
	}

	if (s.keyState[SDL_SCANCODE_W]) {
		r.camera.pos.z += movementSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_S]) {
		r.camera.pos.z -= movementSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_A]) {
		r.camera.pos.x -= movementSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_D]) {
		r.camera.pos.x += movementSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_LCTRL]) {
		r.camera.pos.y += movementSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_SPACE]) {
		r.camera.pos.y -= movementSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_UP]) {
		r.camera.rot.x += turnSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_DOWN]) {
		r.camera.rot.x -= turnSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_LEFT]) {
		r.camera.rot.y -= turnSpeed * s.dt;
		r.restartRender = 1;
	}

	if (s.keyState[SDL_SCANCODE_RIGHT]) {
		r.camera.rot.y += turnSpeed * s.dt;
		r.restartRender = 1;
	}
}

//---- public ----------------------------------------------------------------//

GUIStatus create_window(int width, int height) {
	// window
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return GUI_FAILURE;

	s.window = NULL;
	s.windowSurface = NULL;
	s.renderSurface = NULL;

	s.window = SDL_CreateWindow("pathtracer", SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED, width, height,
								SDL_WINDOW_SHOWN);

	if (s.window == NULL)
		return GUI_FAILURE;

	s.windowSurface = SDL_GetWindowSurface(s.window);

	s.keyState = (Uint8 *)SDL_GetKeyboardState(NULL);

	// font
	if (TTF_Init() < 0)
		return GUI_FAILURE;

	s.font = TTF_OpenFont("data/assets/opensans.ttf", 24);

	if (s.font == NULL)
		return GUI_FAILURE;

	return GUI_SUCCESS;
}

GUIStatus start_main_loop() {
	begin_rendering();

	while (!s.quit) {
		_update_dt();
		_procces_events();
		_process_kb_input();

		if (s.renderSurface != NULL) {
			SDL_FreeSurface(s.renderSurface);
			s.renderSurface = NULL;
		}

		s.renderSurface = _get_render_surface();
		SDL_BlitSurface(s.renderSurface, NULL, s.windowSurface, NULL);

		char fps[100];
		sprintf(fps, "%03d, %03d", (int)(1 / s.dt), (int)(1 / r.dt));
		print_text(fps, 10, 10, (SDL_Color){255, 255, 255},
				   (SDL_Color){0, 0, 0});

		SDL_UpdateWindowSurface(s.window);
	}

	end_rendering();

	return GUI_SUCCESS;
}

GUIStatus close_window() {
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