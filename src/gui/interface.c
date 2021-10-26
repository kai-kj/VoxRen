#include "gui.h"

#define FONT_SIZE 10 * 2
#define AIM_SIZE 5

GUIStatus draw_aim() {
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE, BLACK);
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE / 2, WHITE);

	return GUI_SUCCESS;
}

GUIStatus draw_info_bar() {
	DrawRectangle(FONT_SIZE, FONT_SIZE, GetScreenWidth() - FONT_SIZE * 2, FONT_SIZE * 2, RAYWHITE);

	char text[100];
	sprintf(text, "size %dx%d | fps %03d | voxels %d | chunks %d", r.image.size.x, r.image.size.y, (int)(1.0 / r.dt),
			r.scene.voxelCount, r.scene.chunkCount);
	DrawText(text, FONT_SIZE + FONT_SIZE / 2, FONT_SIZE + FONT_SIZE / 2, FONT_SIZE, BLACK);

	return GUI_SUCCESS;
}