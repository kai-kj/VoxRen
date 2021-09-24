#include "gui.h"

//---- private ---------------------------------------------------------------//

static void _add_letter_to_command(char c) {
	if (!g.firstChar) {
		g.commandLength++;
		g.command = realloc(g.command, g.commandLength);
		g.command[g.commandLength - 2] = c;
		g.command[g.commandLength - 1] = '\0';
	}

	g.firstChar = 0;
}

static void _remove_letter_from_command() {
	if (g.commandLength != 1) {
		g.commandLength--;
		g.command = realloc(g.command, g.commandLength);
		g.command[g.commandLength - 1] = '\0';
	}
}

static void _delete_command() {
	g.commandLength = 1;
	g.command = realloc(g.command, g.commandLength);
	g.command[g.commandLength - 1] = '\0';
}

//---- public ----------------------------------------------------------------//

GUIStatus procces_events() {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				g.quit = 1;
				break;

			case SDL_TEXTINPUT:
				if (g.commandMode) {
					_add_letter_to_command(*e.text.text);
				}
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						if (g.commandMode) {
							g.commandMode = 0;
							_delete_command();
						}
						break;

					case SDLK_RETURN:
						if (g.commandMode) {
							g.commandMode = 0;
							if (g.commandLength != 1)
								run_command(g.command);
							_delete_command();
						}
						break;

					case SDLK_BACKSPACE:
						if (g.commandMode)
							_remove_letter_from_command();
						break;

					case SDLK_q:
						if (!g.commandMode)
							g.quit = 1;
						break;

					case SDLK_p:
						if (!g.commandMode) {
							g.commandMode = 1;
							g.firstChar = 1;
						}
						break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				switch (e.button.button) {
					case SDL_BUTTON_LEFT:
						add_voxel_at_mouse();
						break;

					case SDL_BUTTON_RIGHT:
						g.draging = 1;
						break;
				}

				break;

			case SDL_MOUSEBUTTONUP:
				switch (e.button.button) {
					case SDL_BUTTON_RIGHT:
						g.draging = 0;
						break;
				}

				break;
		}
	}

	return GUI_SUCCESS;
}

GUIStatus process_kb_input() {
	SDL_PumpEvents();

	if (!g.commandMode) {
		if (g.keyState[SDL_SCANCODE_W]) {
			r.camera.pos.z += MOV_SPEED * cos(r.camera.rot.x) * g.dt;
			r.camera.pos.x -= MOV_SPEED * sin(r.camera.rot.x) * g.dt;
			r.restartRender = 1;
		}

		if (g.keyState[SDL_SCANCODE_S]) {
			r.camera.pos.z -= MOV_SPEED * cos(r.camera.rot.x) * g.dt;
			r.camera.pos.x += MOV_SPEED * sin(r.camera.rot.x) * g.dt;
			r.restartRender = 1;
		}

		if (g.keyState[SDL_SCANCODE_A]) {
			r.camera.pos.z -= MOV_SPEED * sin(r.camera.rot.x) * g.dt;
			r.camera.pos.x -= MOV_SPEED * cos(r.camera.rot.x) * g.dt;
			r.restartRender = 1;
		}

		if (g.keyState[SDL_SCANCODE_D]) {
			r.camera.pos.z += MOV_SPEED * sin(r.camera.rot.x) * g.dt;
			r.camera.pos.x += MOV_SPEED * cos(r.camera.rot.x) * g.dt;
			r.restartRender = 1;
		}

		if (g.keyState[SDL_SCANCODE_LCTRL]) {
			r.camera.pos.y += MOV_SPEED * g.dt;
			r.restartRender = 1;
		}

		if (g.keyState[SDL_SCANCODE_SPACE]) {
			r.camera.pos.y -= MOV_SPEED * g.dt;
			r.restartRender = 1;
		}
	}

	return GUI_SUCCESS;
}

GUIStatus precess_mouse_input() {
	if (g.draging) {
		int currentX, currentY;
		SDL_GetMouseState(&currentX, &currentY);

		int deltaX = currentX - g.prevMousePosX;
		int deltaY = currentY - g.prevMousePosY;

		r.camera.rot.x += deltaX * TURN_SPEED * g.dt;
		r.camera.rot.y += deltaY * TURN_SPEED * g.dt;
		r.restartRender = 1;
	}

	return GUI_SUCCESS;
}