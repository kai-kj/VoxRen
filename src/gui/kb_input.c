#include "gui.h"

void _fps_mode() {
	if (IsKeyDown(KEY_W)) {
		r.camera.pos.z += MOV_SPEED * cos(r.camera.rot.x) * GetFrameTime();
		r.camera.pos.x -= MOV_SPEED * sin(r.camera.rot.x) * GetFrameTime();
		r.restartRender = 1;
	}

	if (IsKeyDown(KEY_S)) {
		r.camera.pos.z -= MOV_SPEED * cos(r.camera.rot.x) * GetFrameTime();
		r.camera.pos.x += MOV_SPEED * sin(r.camera.rot.x) * GetFrameTime();
		r.restartRender = 1;
	}

	if (IsKeyDown(KEY_A)) {
		r.camera.pos.z -= MOV_SPEED * sin(r.camera.rot.x) * GetFrameTime();
		r.camera.pos.x -= MOV_SPEED * cos(r.camera.rot.x) * GetFrameTime();
		r.restartRender = 1;
	}

	if (IsKeyDown(KEY_D)) {
		r.camera.pos.z += MOV_SPEED * sin(r.camera.rot.x) * GetFrameTime();
		r.camera.pos.x += MOV_SPEED * cos(r.camera.rot.x) * GetFrameTime();
		r.restartRender = 1;
	}

	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		r.camera.pos.y += MOV_SPEED * GetFrameTime();
		r.restartRender = 1;
	}

	if (IsKeyDown(KEY_SPACE)) {
		r.camera.pos.y -= MOV_SPEED * GetFrameTime();
		r.restartRender = 1;
	}

	if (IsKeyPressed(KEY_E)) {
		if (g.state == 1) {
			HideCursor();
			DisableCursor();

			g.prevMousePosX = GetMouseX();
			g.prevMousePosY = GetMouseY();
		} else {
			ShowCursor();
			EnableCursor();
		}

		g.state = !g.state;
	}

	if (IsKeyDown(KEY_Q)) {
		VoxMaterial *material = get_material_at_mouse();
		if (material != NULL) g.selectedMaterial = *material;
	}
}

void _editor_mode() {
	if (IsKeyPressed(KEY_E)) {
		if (g.state == 1) {
			HideCursor();
			DisableCursor();

			g.prevMousePosX = GetMouseX();
			g.prevMousePosY = GetMouseY();
		} else {
			ShowCursor();
			EnableCursor();
		}

		g.state = !g.state;
	}

	if (!mouse_on_window()) {
		if (IsKeyDown(KEY_Q)) {
			VoxMaterial *material = get_material_at_mouse();
			if (material != NULL) g.selectedMaterial = *material;
		}
	}
}

void _text_input_mode() {
}

RendererStatus procces_kb_input() {
	switch (g.state) {
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

	return GUI_SUCCESS;
}