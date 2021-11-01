#include "gui.h"

void _fps_mode() {
	if (IsKeyDown(KEY_W)) {
		ren.camera.pos.z += gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x -= gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_S)) {
		ren.camera.pos.z -= gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x += gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_A)) {
		ren.camera.pos.z -= gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x -= gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_D)) {
		ren.camera.pos.z += gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
		ren.camera.pos.x += gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		ren.camera.pos.y += gui.cameraMoveSpeed * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_SPACE)) {
		ren.camera.pos.y -= gui.cameraMoveSpeed * GetFrameTime();
		ren.restartRender = 1;
	}

	if (IsKeyDown(KEY_Q)) {
		VoxMaterial *material = get_material_at_mouse();
		if (material != NULL) gui.selectedMaterial = *material;
	}

	if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_ESCAPE)) {
		if (gui.comp.state == 1) {
			HideCursor();
			DisableCursor();

			gui.comp.prevMousePosX = GetMouseX();
			gui.comp.prevMousePosY = GetMouseY();
		} else {
			ShowCursor();
			EnableCursor();
		}

		gui.comp.state = !gui.comp.state;
	}
}

void _editor_mode() {

	if (!mouse_on_window()) {
		if (IsKeyDown(KEY_W)) {
			ren.camera.pos.z += gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
			ren.camera.pos.x -= gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
			ren.restartRender = 1;
		}

		if (IsKeyDown(KEY_S)) {
			ren.camera.pos.z -= gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
			ren.camera.pos.x += gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
			ren.restartRender = 1;
		}

		if (IsKeyDown(KEY_A)) {
			ren.camera.pos.z -= gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
			ren.camera.pos.x -= gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
			ren.restartRender = 1;
		}

		if (IsKeyDown(KEY_D)) {
			ren.camera.pos.z += gui.cameraMoveSpeed * sin(ren.camera.rot.x) * GetFrameTime();
			ren.camera.pos.x += gui.cameraMoveSpeed * cos(ren.camera.rot.x) * GetFrameTime();
			ren.restartRender = 1;
		}

		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			ren.camera.pos.y += gui.cameraMoveSpeed * GetFrameTime();
			ren.restartRender = 1;
		}

		if (IsKeyDown(KEY_SPACE)) {
			ren.camera.pos.y -= gui.cameraMoveSpeed * GetFrameTime();
			ren.restartRender = 1;
		}

		if (IsKeyDown(KEY_Q)) {
			VoxMaterial *material = get_material_at_mouse();
			if (material != NULL) gui.selectedMaterial = *material;
		}
	}

	if (IsKeyPressed(KEY_E)) {
		if (gui.comp.state == 1) {
			HideCursor();
			DisableCursor();

			gui.comp.prevMousePosX = GetMouseX();
			gui.comp.prevMousePosY = GetMouseY();
		} else {
			ShowCursor();
			EnableCursor();
		}

		gui.comp.state = !gui.comp.state;
	}
}

void _text_input_mode() {
}

Status procces_kb_input() {
	switch (gui.comp.state) {
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

	if (IsKeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL)) save_scene_to_file(ren.fileName);

	return SUCCESS;
}