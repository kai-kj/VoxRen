#include "../interface.h"

void _save() {
	save_scene_to_file(ren.fileName);
}

void _create_file_window() {
	ComponentID w = create_window(GetScreenWidth() - 250 - 40, 18 * 40 + 40 + 40, 1, 3, 250, 40, "File");
	add_component(w, 0, 0, create_label("Filename:"));
	ComponentID l = add_component(w, 0, 1, create_label(""));
	change_component_text(l, "  %s", ren.fileName);
	add_component(w, 0, 2, create_button(60, 30, "Save", _save));
}

void _update_file_window() {
}