#include "gui.h"

#define GC_VALUE(v) sqrt(v)
#define FLT_TO_CHAR(f) (char)((f <= 0) ? 0 : (f >= 1) ? 255 : (int)(f * 255))

char *_get_rendered_pixels() {
	char *pixels = malloc(sizeof(char) * r.image.size.x * r.image.size.x * 4);

	for (int i = 0; i < r.image.size.x * r.image.size.y; i++) {
		pixels[i * 4 + 0] = FLT_TO_CHAR(GC_VALUE(r.image.data[i].x));
		pixels[i * 4 + 1] = FLT_TO_CHAR(GC_VALUE(r.image.data[i].y));
		pixels[i * 4 + 2] = FLT_TO_CHAR(GC_VALUE(r.image.data[i].z));
		pixels[i * 4 + 3] = 255;
	}

	return pixels;
}

GUIStatus start_main_loop() {
	msg("Starting main loop");

	begin_rendering();

	while (!WindowShouldClose()) {
		r.readFirstFrame = 1;

		g.mousePosX = GetMouseX();
		g.mousePosY = GetMouseY();

		procces_kb_input();
		procces_mouse_input();

		update_info_window();
		update_material_window();

		update_windows();

		char *pixels = _get_rendered_pixels();
		UpdateTexture(g.renderTexture, pixels);
		free(pixels);

		BeginDrawing();

		ClearBackground(BLACK);

		float scale = min(GetScreenWidth() / r.image.size.x, GetScreenHeight() / r.image.size.y);
		DrawTextureEx(g.renderTexture, (Vector2){(GetScreenWidth() - r.image.size.x * scale) / 2, 0}, 0, scale, WHITE);

		if (g.state == 0) draw_aim();

		draw_windows();

		EndDrawing();

		g.prevMousePosX = g.mousePosX;
		g.prevMousePosY = g.mousePosY;
	}

	return GUI_SUCCESS;
}