#include "interface.h"

Status procces_kb_input() {
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

	if (IsKeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL)) save_scene_to_file(ren.fileName);

	return SUCCESS;
}