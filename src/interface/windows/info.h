#include "../interface.h"

void _draw_mode_indicator(int x, int y) {
	x += 20;
	y -= 5;
	if (gui.removeVoxel) {
		DrawRectangle(x, y, 110, 30, (Color){255, 0, 0, 255});
		DrawRectangleLinesEx((Rectangle){x, y, 110, 30}, 2, (Color){120, 0, 0, 255});
		DrawText("REMOVE", x + 10, y + 5, 20, (Color){120, 0, 0, 255});
	} else {
		DrawRectangle(x, y, 110, 30, (Color){0, 255, 0, 255});
		DrawRectangleLinesEx((Rectangle){x, y, 110, 30}, 2, (Color){0, 120, 0, 255});
		DrawText("ADD", x + 10 + 25, y + 5, 20, (Color){0, 120, 0, 255});
	}
}

void _create_info_window() {
	ComponentID w = create_window(20, 400 + 40 + 40, 1, 15, 250, 40, "Info");

	add_component(w, 0, 0, create_label("Mode"));
	add_component(w, 0, 1, create_custom_component(_draw_mode_indicator));
	add_component(w, 0, 2, create_label("Performance"));
	gui.components.fps = add_component(w, 0, 3, create_label("  FPS:"));
	gui.components.rps = add_component(w, 0, 4, create_label("  RPS:"));
	add_component(w, 0, 5, create_label("Looking At"));
	gui.components.lookingAtPixel = add_component(w, 0, 6, create_label("  Pixel:"));
	gui.components.lookingAtVoxel = add_component(w, 0, 7, create_label("  Voxel:"));
	gui.components.lookingAtNormal = add_component(w, 0, 8, create_label("  Normal:"));
	gui.components.lookingAtDistance = add_component(w, 0, 9, create_label("  Distance:"));
	add_component(w, 0, 10, create_label("Render Image"));
	ComponentID l = add_component(w, 0, 11, create_label("  000x000 px"));
	change_component_text(l, "  %dx%d px", ren.image.size.x, ren.image.size.y);
	add_component(w, 0, 12, create_label("Voxel / Chunk count:"));
	gui.components.voxCount = add_component(w, 0, 13, create_label("  Voxels:"));
	gui.components.chunkCount = add_component(w, 0, 14, create_label("  Chunks:"));
}

void _update_info_window() {
	change_component_text(gui.components.fps, "  FPS: %03d", (int)(GetFPS()));
	change_component_text(gui.components.rps, "  RPS: %03d", (int)(1.0 / ren.dt));
	change_component_text(gui.components.lookingAtPixel, "  Pixel:  (%d, %d)", gui.renderMousePosX,
						  gui.renderMousePosY);
	change_component_text(gui.components.lookingAtVoxel, "  Voxel:  (%d, %d, %d)", ren.lookingAtPos.x,
						  ren.lookingAtPos.y, ren.lookingAtPos.z);
	change_component_text(gui.components.lookingAtNormal, "  Normal: (%d, %d, %d)", ren.lookingAtNormal.x,
						  ren.lookingAtNormal.y, ren.lookingAtNormal.z);
	change_component_text(gui.components.lookingAtDistance, "  Distance: %.02f", ren.lookingAtDistance);
	change_component_text(gui.components.voxCount, "  Voxels: %d", ren.scene.voxelCount);
	change_component_text(gui.components.chunkCount, "  Chunks: %d", ren.scene.chunkCount);
}