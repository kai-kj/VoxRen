#include "interface.h"

#define GC_VALUE(v) sqrt(v)
#define FLT_TO_CHAR(f) (char)((f <= 0) ? 0 : (f >= 1) ? 255 : (int)(f * 255))

char *_get_rendered_pixels() {
	char *pixels = malloc(ren.image.size.x * ren.image.size.y * 4);

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

		procces_kb_input();
		procces_mouse_input();
		process_gui();

		update_interface();

		// update render image
		char *pixels = _get_rendered_pixels();
		UpdateTexture(gui.renderTexture, pixels);
		free(pixels);

		BeginDrawing();
		ClearBackground(BLACK);

		// draw render image
		float scale = min((float)GetScreenWidth() / ren.image.size.x, (float)GetScreenHeight() / ren.image.size.y);
		int xPos = (GetScreenWidth() - ren.image.size.x * scale) / 2;
		int yPos = (GetScreenHeight() - ren.image.size.y * scale) / 2;
		DrawTextureEx(gui.renderTexture, (Vector2){xPos, yPos}, 0, scale, WHITE);

		// draw gui
		draw_gui();
		draw_additional_interface();

		EndDrawing();
	}

	end_rendering();

	return SUCCESS;
}