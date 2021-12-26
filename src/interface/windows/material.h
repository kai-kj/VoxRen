#include "../interface.h"

#define COLOR_STEP 15
#define DETAILS_STEP 0.1

void _r_plus() {
	gui.selectedMaterial.color.x += 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.x > 1) gui.selectedMaterial.color.x = 1;
}

void _r_minus() {
	gui.selectedMaterial.color.x -= 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.x < 0) gui.selectedMaterial.color.x = 0;
}

void _g_plus() {
	gui.selectedMaterial.color.y += 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.y > 1) gui.selectedMaterial.color.y = 1;
}

void _g_minus() {
	gui.selectedMaterial.color.y -= 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.y < 0) gui.selectedMaterial.color.y = 0;
}

void _b_plus() {
	gui.selectedMaterial.color.z += 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.z > 1) gui.selectedMaterial.color.z = 1;
}

void _b_minus() {
	gui.selectedMaterial.color.z -= 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.z < 0) gui.selectedMaterial.color.z = 0;
}

void _set_r(char *text) {
	float color = (float)atoi(text) / 255.0;
	gui.selectedMaterial.color.x = color > 1 ? 1 : color < 0 ? 0 : color;
}

void _set_g(char *text) {
	float color = (float)atoi(text) / 255.0;
	gui.selectedMaterial.color.y = color > 1 ? 1 : color < 0 ? 0 : color;
}

void _set_b(char *text) {
	float color = (float)atoi(text) / 255.0;
	gui.selectedMaterial.color.z = color > 1 ? 1 : color < 0 ? 0 : color;
}

void _next_material() {
	gui.selectedMaterial.type++;
	if (gui.selectedMaterial.type > 4) gui.selectedMaterial.type = 1;
}

void _prev_material() {
	gui.selectedMaterial.type++;
	if (gui.selectedMaterial.type < 1) gui.selectedMaterial.type = 4;
}

void _set_v1(char *text) {
	switch (gui.selectedMaterial.type) {
		case 1: gui.selectedMaterial.v1 = max(0, atof(text)); break;
		case 2: break;
		case 3: gui.selectedMaterial.v1 = min(max(0, atof(text)), 1); break;
		case 4: gui.selectedMaterial.v1 = min(max(0, atof(text)), 1); break;
	}
}

void _set_v2(char *text) {
	switch (gui.selectedMaterial.type) {
		case 1: break;
		case 2: break;
		case 3: gui.selectedMaterial.v2 = min(max(0, atof(text)), 1); break;
		case 4: gui.selectedMaterial.v2 = min(max(0, atof(text)), 1); break;
	}
}

void _set_v3(char *text) {
	switch (gui.selectedMaterial.type) {
		case 1: break;
		case 2: break;
		case 3: break;
		case 4: gui.selectedMaterial.v3 = min(max(0, atof(text)), 1); break;
	}
}

void _draw_color_indicator(int x, int y) {
	int r = gui.selectedMaterial.color.x * 255;
	int g = gui.selectedMaterial.color.y * 255;
	int b = gui.selectedMaterial.color.z * 255;
	DrawRectangle(x, y, 110, 20, (Color){r, g, b, 255});
	DrawRectangleLinesEx((Rectangle){x, y, 110, 20}, 2, DARKGRAY);
}

void _create_material_window() {
	ComponentID w = create_window(20, 20, 1, 10, 250, 40, "Material");
	ComponentID g;

	g = add_component(w, 0, 0, create_grid(2, 1, 120, 20));
	add_component(g, 0, 0, create_label("Color"));
	add_component(g, 1, 0, create_custom_component(_draw_color_indicator));

	g = add_component(w, 0, 1, create_grid(4, 1, 60, 20));
	add_component(g, 0, 0, create_label("  R:"));
	add_component(g, 1, 0, create_button(50, 25, "-", _r_minus));
	gui.components.red = add_component(g, 2, 0, create_textbox(50, 25, "000", _set_r));
	add_component(g, 3, 0, create_button(50, 25, "+", _r_plus));

	g = add_component(w, 0, 2, create_grid(4, 1, 60, 20));
	add_component(g, 0, 0, create_label("  G:"));
	add_component(g, 1, 0, create_button(50, 25, "-", _g_minus));
	gui.components.green = add_component(g, 2, 0, create_textbox(50, 25, "000", _set_g));
	add_component(g, 3, 0, create_button(50, 25, "+", _g_plus));

	g = add_component(w, 0, 3, create_grid(4, 1, 60, 20));
	add_component(g, 0, 0, create_label("  B:"));
	add_component(g, 1, 0, create_button(50, 25, "-", _b_minus));
	gui.components.blue = add_component(g, 2, 0, create_textbox(50, 25, "000", _set_b));
	add_component(g, 3, 0, create_button(50, 25, "+", _b_plus));

	add_component(w, 0, 4, create_label("Type"));

	g = add_component(w, 0, 5, create_grid(4, 1, 60, 20));
	add_component(g, 0, 0, create_button(50, 25, "<", _prev_material));
	gui.components.material = add_component(g, 1, 0, create_label("Lambert"));
	add_component(g, 3, 0, create_button(50, 25, ">", _next_material));

	add_component(w, 0, 6, create_label("Properties"));

	g = add_component(w, 0, 7, create_grid(2, 1, 150, 20));
	gui.components.v1Tag = add_component(g, 0, 0, create_label("  v1:"));
	gui.components.v1 = add_component(g, 1, 0, create_textbox(80, 25, "0.0", _set_v1));

	g = add_component(w, 0, 8, create_grid(2, 1, 150, 20));
	gui.components.v2Tag = add_component(g, 0, 0, create_label("  v2:"));
	gui.components.v2 = add_component(g, 1, 0, create_textbox(80, 25, "0.0", _set_v2));

	g = add_component(w, 0, 9, create_grid(2, 1, 150, 20));
	gui.components.v3Tag = add_component(g, 0, 0, create_label("  v3:"));
	gui.components.v3 = add_component(g, 1, 0, create_textbox(80, 25, "0.0", _set_v3));
}

void _update_material_window() {
	change_component_text(gui.components.red, "%03d", (int)(gui.selectedMaterial.color.x * 255));
	change_component_text(gui.components.green, "%03d", (int)(gui.selectedMaterial.color.y * 255));
	change_component_text(gui.components.blue, "%03d", (int)(gui.selectedMaterial.color.z * 255));
	switch (gui.selectedMaterial.type) {
		case 1:
			change_component_text(gui.components.material, "%s", "light");
			change_component_text(gui.components.v1Tag, "  brightness:");
			change_component_text(gui.components.v2Tag, "");
			change_component_text(gui.components.v3Tag, "");
			change_component_text(gui.components.v1, "%.02f", gui.selectedMaterial.v1);
			change_component_text(gui.components.v2, "--");
			change_component_text(gui.components.v3, "--");
			break;
		case 2:
			change_component_text(gui.components.material, "%s", "lambert");
			change_component_text(gui.components.v1Tag, "");
			change_component_text(gui.components.v2Tag, "");
			change_component_text(gui.components.v3Tag, "");
			change_component_text(gui.components.v1, "--");
			change_component_text(gui.components.v2, "--");
			change_component_text(gui.components.v3, "--");
			break;
		case 3:
			change_component_text(gui.components.material, "%s", "metal");
			change_component_text(gui.components.v1Tag, "  tint:");
			change_component_text(gui.components.v2Tag, "  fuzz:");
			change_component_text(gui.components.v3Tag, "");
			change_component_text(gui.components.v1, "%.02f", gui.selectedMaterial.v1);
			change_component_text(gui.components.v2, "%.02f", gui.selectedMaterial.v2);
			change_component_text(gui.components.v3, "--");
			break;
		case 4:
			change_component_text(gui.components.material, "%s", "glass");
			change_component_text(gui.components.v1Tag, "  tint:");
			change_component_text(gui.components.v2Tag, "  fuzz:");
			change_component_text(gui.components.v3Tag, "  refIdx:");
			change_component_text(gui.components.v1, "%.02f", gui.selectedMaterial.v1);
			change_component_text(gui.components.v2, "%.02f", gui.selectedMaterial.v2);
			change_component_text(gui.components.v3, "%.02f", gui.selectedMaterial.v3);
			break;
	}
}