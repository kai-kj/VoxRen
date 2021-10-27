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

	if (IsKeyPressed(KEY_E)) {
		if (g.state == 1) {
			HideCursor();
			DisableCursor();

			g.prevMousePosX = GetMouseX();
			g.prevMousePosY = GetMouseY();
		} else {
			ShowCursor();
			EnableCursor();
		}

		g.state = !g.state;
	}
}

void _procces_mouse_input() {
	if (g.state == 0) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			add_voxel_at_mouse(g.selectedMaterial);
			r.restartRender = 1;
		}

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			remove_voxel_at_mouse();
			r.restartRender = 1;
		}

		if (g.mousePosX != g.prevMousePosX || g.mousePosY != g.prevMousePosY) {
			int deltaX = g.mousePosX - g.prevMousePosX;
			int deltaY = g.mousePosY - g.prevMousePosY;

			r.camera.rot.x -= deltaX * TURN_SPEED * GetFrameTime();
			r.camera.rot.y -= deltaY * TURN_SPEED * GetFrameTime();

			r.restartRender = 1;
		}
	} else if (g.state == 1) {
		if (!mouse_on_window()) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				add_voxel_at_mouse(g.selectedMaterial);
				r.restartRender = 1;
			}

			if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
				remove_voxel_at_mouse();
				r.restartRender = 1;
			}
		}
	}

	int prevRenderMousePosX = g.renderMousePosX;
	int prevRenderMousePosY = g.renderMousePosY;
	float scale = min(GetScreenWidth() / r.image.size.x, GetScreenHeight() / r.image.size.y);

	if (g.state == 1) {
		g.renderMousePosX = (g.mousePosX - (GetScreenWidth() - r.image.size.x * scale) / 2) / scale;
		g.renderMousePosY = (g.mousePosY - (GetScreenHeight() - r.image.size.y * scale) / 2) / scale;
	} else {
		g.renderMousePosX = r.image.size.x / 2;
		g.renderMousePosY = r.image.size.y / 2;
	}

	if (g.renderMousePosX != prevRenderMousePosX || g.renderMousePosY != prevRenderMousePosY) r.restartRender = 1;

	set_mouse_pos(g.renderMousePosX, g.renderMousePosY);
}

GUIStatus start_main_loop() {
	msg("Starting main loop");

	begin_rendering();

	while (!WindowShouldClose()) {
		r.readFirstFrame = 1;

		g.mousePosX = GetMouseX();
		g.mousePosY = GetMouseY();

		_procces_kb_input();
		_procces_mouse_input();

		char *pixels = _get_rendered_pixels();
		UpdateTexture(g.renderTexture, pixels);
		free(pixels);

		BeginDrawing();

		ClearBackground(BLACK);

		float scale = min(GetScreenWidth() / r.image.size.x, GetScreenHeight() / r.image.size.y);
		DrawTextureEx(g.renderTexture, (Vector2){(GetScreenWidth() - r.image.size.x * scale) / 2, 0}, 0, scale, WHITE);

		update_info_window();
		update_material_window();

		if (g.state == 0) draw_aim();

		update_windows();
		draw_windows();

		EndDrawing();

		g.prevMousePosX = g.mousePosX;
		g.prevMousePosY = g.mousePosY;
	}

	return GUI_SUCCESS;
}