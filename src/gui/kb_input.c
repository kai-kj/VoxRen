#include "gui.h"

void _fps_mode() {
	if (IsKeyDown(KEY_W)) {
		ren.camera.pos.z += MOV_SPEED * cos(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x -= MOV_SPEED * sin(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_S)) {
		ren.camera.pos.z -= MOV_SPEED * cos(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x += MOV_SPEED * sin(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_A)) {
		ren.camera.pos.z -= MOV_SPEED * sin(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x -= MOV_SPEED * cos(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_D)) {
		ren.camera.pos.z += MOV_SPEED * sin(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x += MOV_SPEED * cos(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		ren.camera.pos.y += MOV_SPEED * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_SPACE)) {
		ren.camera.pos.y -= MOV_SPEED * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyPressed(KEY_E)) {
		if (gui.state == 1) {
			HideCursor();
			DisableCursor();

			gui.comp.prevMousePosX = GetMouseX();
			gui.comp.prevMousePosY = GetMouseY();
		} else {
			ShowCursor();
			EnableCursor();
		}

		gui.state = !gui.state;
	}

	if (IsKeyDown(KEY_Q)) {
		VoxMaterial *material = get_material_at_mouse();
		if (material != NULL) gui.selectedMaterial = *material;
	}
}

void _editor_mode() {
	if (IsKeyPressed(KEY_E)) {
		if (gui.state == 1) {
			HideCursor();
			DisableCursor();

			gui.comp.prevMousePosX = GetMouseX();
			gui.comp.prevMousePosY = GetMouseY();
		} else {
			ShowCursor();
			EnableCursor();
		}

		gui.state = !gui.state;
	}

	if (!mouse_on_window()) {
		if (IsKeyDown(KEY_Q)) {
			VoxMaterial *material = get_material_at_mouse();
			if (material != NULL) gui.selectedMaterial = *material;
		}
	}
}

void _text_input_mode() {
}

Status procces_kb_input() {
	switch (gui.state) {
		case 0:
			_fps_mode();
			break;

		case 1:
			_editor_mode();
			break;

		case 2:
			_text_input_mode();
			break;
	}

	return SUCCESS;
}