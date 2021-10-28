#include "gui.h"

static void _fps_mode() {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		add_voxel_at_mouse(g.selectedMaterial);
		r.restartRender = 1;
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
		remove_voxel_at_mouse();
		r.restartRender = 1;
	}

	if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
		VoxMaterial *material = get_material_at_mouse();
		if (material != NULL) g.selectedMaterial = *material;
	}

	if (g.mousePosX != g.prevMousePosX || g.mousePosY != g.prevMousePosY) {
		int deltaX = g.mousePosX - g.prevMousePosX;
		int deltaY = g.mousePosY - g.prevMousePosY;

		r.camera.rot.x -= deltaX * TURN_SPEED * GetFrameTime();
		r.camera.rot.y -= deltaY * TURN_SPEED * GetFrameTime();

		r.restartRender = 1;
	}

	g.renderMousePosX = r.image.size.x / 2;
	g.renderMousePosY = r.image.size.y / 2;
}

static void _editor_mode() {
	if (!mouse_on_window()) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			add_voxel_at_mouse(g.selectedMaterial);
			r.restartRender = 1;
		}

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			remove_voxel_at_mouse();
			r.restartRender = 1;
		}

		if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
			VoxMaterial *material = get_material_at_mouse();
			if (material != NULL) g.selectedMaterial = *material;
		}
	}

	float scale = min(GetScreenWidth() / r.image.size.x, GetScreenHeight() / r.image.size.y);
	g.renderMousePosX = (g.mousePosX - (GetScreenWidth() - r.image.size.x * scale) / 2) / scale;
	g.renderMousePosY = (g.mousePosY - (GetScreenHeight() - r.image.size.y * scale) / 2) / scale;
}

static void _text_input_mode() {
}

GUIStatus procces_mouse_input() {
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

	set_mouse_pos(g.renderMousePosX, g.renderMousePosY);

	return GUI_SUCCESS;
}
