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

void _procces_kb_input() {
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
}

void _procces_mouse_input() {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		add_voxel_at_mouse(create_lambertian_material(1, 1, 1));
		r.restartRender = 1;
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
		remove_voxel_at_mouse();
		r.restartRender = 1;
	}

	int currentX = GetMouseX();
	int currentY = GetMouseY();

	if (currentX != g.prevMousePosX || currentY != g.prevMousePosY) {
		int deltaX = currentX - g.prevMousePosX;
		int deltaY = currentY - g.prevMousePosY;

		r.camera.rot.x -= deltaX * TURN_SPEED * GetFrameTime();
		r.camera.rot.y -= deltaY * TURN_SPEED * GetFrameTime();
		r.restartRender = 1;
	}

	g.prevMousePosX = GetMouseX();
	g.prevMousePosY = GetMouseY();
}

GUIStatus start_main_loop() {
	msg("Starting main loop");

	begin_rendering();

	while (!WindowShouldClose()) {
		r.readFirstFrame = 1;

		_procces_kb_input();
		_procces_mouse_input();

		char *pixels = _get_rendered_pixels();
		UpdateTexture(g.renderTexture, pixels);
		free(pixels);

		BeginDrawing();

		ClearBackground(BLACK);

		float scale = min(GetScreenWidth() / r.image.size.x, GetScreenHeight() / r.image.size.y);

		DrawTextureEx(g.renderTexture, (Vector2){(GetScreenWidth() - r.image.size.x * scale) / 2, 0}, 0, scale, WHITE);

		draw_aim();
		draw_info_bar();

		EndDrawing();
	}

	return GUI_SUCCESS;
}