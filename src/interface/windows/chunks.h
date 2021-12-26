#include "../interface.h"

void _set_chunk_size(char *text) {
	resize_chunks(atoi(text));
}

void _increase_chunk_size() {
	resize_chunks(ren.scene.chunkSize + 1);
}

void _decrease_chunk_size() {
	resize_chunks(ren.scene.chunkSize - 1);
}

void _create_chunks_window() {
	ComponentID w = create_window(GetScreenWidth() - 250 - 40, (18 + 2) * 40 + 40 * 4 + 20, 1, 2, 250, 40, "Settings");
	add_component(w, 0, 0, create_label("Chunk size:"));
	ComponentID g = add_component(w, 0, 1, create_grid(3, 1, 70, 30));
	add_component(g, 0, 0, create_button(60, 25, "<", _decrease_chunk_size));
	gui.components.chunkSize = add_component(g, 1, 0, create_textbox(60, 25, "", _set_chunk_size));
	add_component(g, 2, 0, create_button(60, 25, ">", _increase_chunk_size));
}

void _update_chunks_window() {
	change_component_text(gui.components.chunkSize, "%d", ren.scene.chunkSize);
}