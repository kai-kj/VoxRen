#include "interface.h"

void _update_camera_direction() {
	if (GetMouseX() != get_prev_mouse_x() || GetMouseY() != get_prev_mouse_y()) {
		int deltaX = GetMouseX() - get_prev_mouse_x();
		int deltaY = GetMouseY() - get_prev_mouse_y();

		float rx = ren.camera.rot.x + deltaX * gui.cameraLookSpeed * GetFrameTime();
		float ry = ren.camera.rot.y + deltaY * gui.cameraLookSpeed * GetFrameTime();

		set_camera_pos(ren.camera.pos.x, ren.camera.pos.y, ren.camera.pos.z, rx, ry);

		ren.restartRender = 1;
	}
}

Status procces_mouse_input() {
	if (is_editing()) return FAILURE;

	if (!mouse_on_windows()) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			gui.removeVoxel ? remove_voxel_at_mouse() : add_voxel_at_mouse(gui.selectedMaterial);
			ren.restartRender = 1;
		}

		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			_update_camera_direction();
		}

		if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
			VoxMaterial *material = get_material_at_mouse();
			if (material != NULL) gui.selectedMaterial = *material;
		}
	}

	float scale = min((float)GetScreenWidth() / ren.image.size.x, (float)GetScreenHeight() / ren.image.size.y);
	gui.renderMousePosX = (GetMouseX() - (GetScreenWidth() - ren.image.size.x * scale) / 2) / scale;
	gui.renderMousePosY = (GetMouseY() - (GetScreenHeight() - ren.image.size.y * scale) / 2) / scale;

	set_mouse_pos(gui.renderMousePosX, gui.renderMousePosY);

	return SUCCESS;
}
