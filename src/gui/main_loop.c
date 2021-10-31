#include "gui.h"

#define GC_VALUE(v) sqrt(v)
#define FLT_TO_CHAR(f) (char)((f <= 0) ? 0 : (f >= 1) ? 255 : (int)(f * 255))

char *_get_rendered_pixels() {
	char *pixels = malloc(sizeof(char) * ren.image.size.x * ren.image.size.y * 4);

	for (int i = 0; i < ren.image.size.x * ren.image.size.y; i++) {
		pixels[i * 4 + 0] = FLT_TO_CHAR(GC_VALUE(ren.image.data[i].x));
		pixels[i * 4 + 1] = FLT_TO_CHAR(GC_VALUE(ren.image.data[i].y));
		pixels[i * 4 + 2] = FLT_TO_CHAR(GC_VALUE(ren.image.data[i].z));
		pixels[i * 4 + 3] = 255;
	}

	return pixels;
}

Status start_main_loop() {
	msg("Starting main loop");

	begin_rendering();

	while (!WindowShouldClose()) {
		ren.readFirstFrame = 1;

		gui.comp.mousePosX = GetMouseX();
		gui.comp.mousePosY = GetMouseY();

		procces_kb_input();
		procces_mouse_input();

		update_gui_components();
		update_windows();

		char *pixels = _get_rendered_pixels();
		UpdateTexture(gui.renderTexture, pixels);
		free(pixels);

		BeginDrawing();

		ClearBackground(BLACK);

		float scale = min(GetScreenWidth() / ren.image.size.x, GetScreenHeight() / ren.image.size.y);
		DrawTextureEx(gui.renderTexture, (Vector2){(GetScreenWidth() - ren.image.size.x * scale) / 2, 0}, 0, scale,
					  WHITE);

		if (gui.comp.state == 0) draw_aim();

		draw_gui_components();

		EndDrawing();

		gui.comp.prevMousePosX = gui.comp.mousePosX;
		gui.comp.prevMousePosY = gui.comp.mousePosY;
	}

	return SUCCESS;
}