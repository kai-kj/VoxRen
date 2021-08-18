#include "gui.h"

//---- private ---------------------------------------------------------------//

static void _add_letter_to_command(char c) {
	if (!s.firstChar) {
		s.commandLength++;
		s.command = realloc(s.command, s.commandLength);
		s.command[s.commandLength - 2] = c;
		s.command[s.commandLength - 1] = '\0';
	}

	s.firstChar = 0;
}

static void _remove_letter_from_command() {
	if (s.commandLength != 1) {
		s.commandLength--;
		s.command = realloc(s.command, s.commandLength);
		s.command[s.commandLength - 1] = '\0';
	}
}

static void _delete_command() {
	s.commandLength = 1;
	s.command = realloc(s.command, s.commandLength);
	s.command[s.commandLength - 1] = '\0';
}

//---- public ----------------------------------------------------------------//

GUIStatus procces_events() {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				s.quit = 1;
				break;

			case SDL_TEXTINPUT:
				if (s.commandMode) {
					_add_letter_to_command(*e.text.text);
				}
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						if (s.commandMode) {
							s.commandMode = 0;
							_delete_command();
						}
						break;

					case SDLK_RETURN:
						if (s.commandMode) {
							s.commandMode = 0;
							if (s.commandLength != 1)
								run_command(s.command);
							_delete_command();
						}
						break;

					case SDLK_BACKSPACE:
						if (s.commandMode)
							_remove_letter_from_command();
						break;

					case SDLK_q:
						if (!s.commandMode)
							s.quit = 1;
						break;

					case SDLK_p:
						if (!s.commandMode) {
							s.commandMode = 1;
							s.firstChar = 1;
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
						s.draging = 1;
						break;
				}

				break;

			case SDL_MOUSEBUTTONUP:
				switch (e.button.button) {
					case SDL_BUTTON_RIGHT:
						s.draging = 0;
						break;
				}

				break;
		}
	}

	return GUI_SUCCESS;
}

GUIStatus process_kb_input() {
	SDL_PumpEvents();

	if (!s.commandMode) {
		if (s.keyState[SDL_SCANCODE_W]) {
			r.camera.pos.z += MOV_SPEED * cos(r.camera.rot.x) * s.dt;
			r.camera.pos.x -= MOV_SPEED * sin(r.camera.rot.x) * s.dt;
			r.restartRender = 1;
		}

		if (s.keyState[SDL_SCANCODE_S]) {
			r.camera.pos.z -= MOV_SPEED * cos(r.camera.rot.x) * s.dt;
			r.camera.pos.x += MOV_SPEED * sin(r.camera.rot.x) * s.dt;
			r.restartRender = 1;
		}

		if (s.keyState[SDL_SCANCODE_A]) {
			r.camera.pos.z -= MOV_SPEED * sin(r.camera.rot.x) * s.dt;
			r.camera.pos.x -= MOV_SPEED * cos(r.camera.rot.x) * s.dt;
			r.restartRender = 1;
		}

		if (s.keyState[SDL_SCANCODE_D]) {
			r.camera.pos.z += MOV_SPEED * sin(r.camera.rot.x) * s.dt;
			r.camera.pos.x += MOV_SPEED * cos(r.camera.rot.x) * s.dt;
			r.restartRender = 1;
		}

		if (s.keyState[SDL_SCANCODE_LCTRL]) {
			r.camera.pos.y += MOV_SPEED * s.dt;
			r.restartRender = 1;
		}

		if (s.keyState[SDL_SCANCODE_SPACE]) {
			r.camera.pos.y -= MOV_SPEED * s.dt;
			r.restartRender = 1;
		}
	}

	return GUI_SUCCESS;
}

GUIStatus precess_mouse_input() {
	if (s.draging) {
		int currentX, currentY;
		SDL_GetMouseState(&currentX, &currentY);

		int deltaX = currentX - s.prevMousePosX;
		int deltaY = currentY - s.prevMousePosY;

		r.camera.rot.x += deltaX * TURN_SPEED * s.dt;
		r.camera.rot.y += deltaY * TURN_SPEED * s.dt;
		r.restartRender = 1;
	}

	return GUI_SUCCESS;
}