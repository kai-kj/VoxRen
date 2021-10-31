#include "gui.h"

static void _fps_mode() {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		add_voxel_at_mouse(gui.selectedMaterial);
		ren.restartRender = 1;
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
		remove_voxel_at_mouse();
		ren.restartRender = 1;
	}

	if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
		VoxMaterial *material = get_material_at_mouse();
		if (material != NULL) gui.selectedMaterial = *material;
	}

	if (gui.comp.mousePosX != gui.comp.prevMousePosX || gui.comp.mousePosY != gui.comp.prevMousePosY) {
		int deltaX = gui.comp.mousePosX - gui.comp.prevMousePosX;
		int deltaY = gui.comp.mousePosY - gui.comp.prevMousePosY;

		ren.camera.rot.x -= deltaX * gui.cameraLookSpeed * GetFrameTime();
		ren.camera.rot.y -= deltaY * gui.cameraLookSpeed * GetFrameTime();

		ren.restartRender = 1;
	}

	gui.renderMousePosX = ren.image.size.x / 2;
	gui.renderMousePosY = ren.image.size.y / 2;
}

static void _editor_mode() {
	if (!mouse_on_window()) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			add_voxel_at_mouse(gui.selectedMaterial);
			ren.restartRender = 1;
		}

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			remove_voxel_at_mouse();
			ren.restartRender = 1;
		}

		if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
			VoxMaterial *material = get_material_at_mouse();
			if (material != NULL) gui.selectedMaterial = *material;
		}
	}

	float scale = min(GetScreenWidth() / ren.image.size.x, GetScreenHeight() / ren.image.size.y);
	gui.renderMousePosX = (gui.comp.mousePosX - (GetScreenWidth() - ren.image.size.x * scale) / 2) / scale;
	gui.renderMousePosY = (gui.comp.mousePosY - (GetScreenHeight() - ren.image.size.y * scale) / 2) / scale;
}

static void _text_input_mode() {
}

Status procces_mouse_input() {
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

	set_mouse_pos(gui.renderMousePosX, gui.renderMousePosY);

	return SUCCESS;
}
