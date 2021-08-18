#include "gui.h"

//---- public ----------------------------------------------------------------//

GUIStatus procces_events() {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			s.quit = 1;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_RIGHT) {
				s.rightMouseDown = 1;
			}
		}

		if (e.type == SDL_MOUSEBUTTONUP) {
			if (e.button.button == SDL_BUTTON_RIGHT) {
				s.rightMouseDown = 0;
			}
		}

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				add_voxel_at_mouse();
			}
		}
	}

	return GUI_SUCCESS;
}

GUIStatus process_kb_input() {
	SDL_PumpEvents();

	if (s.keyState[SDL_SCANCODE_Q]) {
		s.quit = 1;
	}

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

	return GUI_SUCCESS;
}

GUIStatus precess_mouse_input() {
	if (s.rightMouseDown) {
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