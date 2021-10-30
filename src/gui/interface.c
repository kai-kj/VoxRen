#include "gui.h"

#define FONT_SIZE 10 * 2
#define AIM_SIZE 5
#define COLOR_STEP 15
#define DETAILS_STEP 0.1

void _r_plus() {
	gui.selectedMaterial.color.x += 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.x > 1) gui.selectedMaterial.color.x = 1;
	msg("%f", gui.selectedMaterial.color.x);
}

void _r_minus() {
	gui.selectedMaterial.color.x -= 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.x < 0) gui.selectedMaterial.color.x = 0;
	msg("%f", gui.selectedMaterial.color.x);
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

void _change_material() {
	switch (gui.selectedMaterial.type) {
		case 1:
			gui.selectedMaterial.v1 = 2;
			gui.selectedMaterial.v2 = 0;
			gui.selectedMaterial.v3 = 0;
			break;

		case 2:
			gui.selectedMaterial.v1 = 0;
			gui.selectedMaterial.v2 = 0;
			gui.selectedMaterial.v3 = 0;
			break;

		case 3:
			gui.selectedMaterial.v1 = 1;
			gui.selectedMaterial.v2 = 0;
			gui.selectedMaterial.v3 = 0;
			break;

		case 4:
			gui.selectedMaterial.v1 = 1;
			gui.selectedMaterial.v2 = 0;
			gui.selectedMaterial.v3 = 1;
			break;
	}
}

void _mat_plus() {
	gui.selectedMaterial.type++;
	if (gui.selectedMaterial.type > 4) gui.selectedMaterial.type = 1;
	_change_material();
}

void _mat_minus() {
	gui.selectedMaterial.type--;
	if (gui.selectedMaterial.type < 1) gui.selectedMaterial.type = 4;
	_change_material();
}

void _v1_plus() {
	gui.selectedMaterial.v1 += 1.0f * DETAILS_STEP;
	if (gui.selectedMaterial.v1 > 1 && gui.selectedMaterial.type != 1) gui.selectedMaterial.v1 = 1;
}

void _v1_minus() {
	gui.selectedMaterial.v1 -= 1.0f * DETAILS_STEP;
	if (gui.selectedMaterial.v1 < 0) gui.selectedMaterial.v1 = 0;
}

void _v2_plus() {
	gui.selectedMaterial.v2 += 1.0f * DETAILS_STEP;
	if (gui.selectedMaterial.v2 > 1) gui.selectedMaterial.v2 = 1;
}

void _v2_minus() {
	gui.selectedMaterial.v2 -= 1.0f * DETAILS_STEP;
	if (gui.selectedMaterial.v2 < 0) gui.selectedMaterial.v2 = 0;
}

void _v3_plus() {
	gui.selectedMaterial.v3 += 1.0f * DETAILS_STEP;
	if (gui.selectedMaterial.v3 > 1) gui.selectedMaterial.v3 = 1;
}

void _v3_minus() {
	gui.selectedMaterial.v3 -= 1.0f * DETAILS_STEP;
	if (gui.selectedMaterial.v3 < 0) gui.selectedMaterial.v3 = 0;
}

void _set_r(char *text) {
	float color = (float)atoi(text) / 255.0;
	if (color > 1) color = 1;
	if (color < 0) color = 0;

	gui.selectedMaterial.color.x = color;
}

void _set_g(char *text) {
	float color = (float)atoi(text) / 255.0;
	if (color > 1) color = 1;
	if (color < 0) color = 0;

	gui.selectedMaterial.color.y = color;
}

void _set_b(char *text) {
	float color = (float)atoi(text) / 255.0;
	if (color > 1) color = 1;
	if (color < 0) color = 0;

	gui.selectedMaterial.color.z = color;
}

void _set_v1(char *text) {
	float z = atof(text);
	if (z < 0) z = 0;

	gui.selectedMaterial.v1 = z;
}

void _set_v2(char *text) {
	float z = atof(text);
	if (z < 0) z = 0;

	gui.selectedMaterial.v2 = z;
}

void _set_v3(char *text) {
	float z = atof(text);
	if (z < 0) z = 0;

	gui.selectedMaterial.v3 = z;
}

Status draw_aim() {
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE, BLACK);
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE / 2, WHITE);

	return SUCCESS;
}

Status update_info_window() {
	char text[1000];
	sprintf(text, "fps: %03d\nscene: %dv, %dc\nlooking at:\n  v(%d, %d, %d)\n  p(%d, %d)", (int)(1.0 / ren.dt),
			ren.scene.voxelCount, ren.scene.chunkCount, ren.lookingAtPos.x, ren.lookingAtPos.y, ren.lookingAtPos.z,
			ren.mousePos.x, ren.mousePos.y);
	change_texbox_text(gui.infoWindow, 0, text);

	return SUCCESS;
}

Status update_material_window() {
	char text[100];

	sprintf(text, "%03d", (int)(gui.selectedMaterial.color.x * 255));
	change_text_input_text(gui.materialWindow, 0, text);

	sprintf(text, "%03d", (int)(gui.selectedMaterial.color.y * 255));
	change_text_input_text(gui.materialWindow, 1, text);

	sprintf(text, "%03d", (int)(gui.selectedMaterial.color.z * 255));
	change_text_input_text(gui.materialWindow, 2, text);

	switch (gui.selectedMaterial.type) {
		case 1:
			sprintf(text, "%s", "Light Source");
			break;
		case 2:
			sprintf(text, "%s", "Lambertian");
			break;
		case 3:
			sprintf(text, "%s", "Metal");
			break;
		case 4:
			sprintf(text, "%s", "Dielectric");
			break;
	}

	change_texbox_text(gui.materialWindow, 5, text);

	switch (gui.selectedMaterial.type) {
		case 1:
			sprintf(text, "%s", "brightness");
			change_texbox_text(gui.materialWindow, 7, text);

			sprintf(text, "(none)");
			change_texbox_text(gui.materialWindow, 8, text);

			sprintf(text, "(none)");
			change_texbox_text(gui.materialWindow, 9, text);

			sprintf(text, "%01.2f", gui.selectedMaterial.v1);
			change_text_input_text(gui.materialWindow, 3, text);

			sprintf(text, " ");
			change_text_input_text(gui.materialWindow, 4, text);

			sprintf(text, " ");
			change_text_input_text(gui.materialWindow, 5, text);

			break;

		case 2:
			sprintf(text, "(none)");
			change_texbox_text(gui.materialWindow, 7, text);

			sprintf(text, "(none)");
			change_texbox_text(gui.materialWindow, 8, text);

			sprintf(text, "(none)");
			change_texbox_text(gui.materialWindow, 9, text);

			sprintf(text, " ");
			change_text_input_text(gui.materialWindow, 3, text);

			sprintf(text, " ");
			change_text_input_text(gui.materialWindow, 4, text);

			sprintf(text, " ");
			change_text_input_text(gui.materialWindow, 5, text);

			break;

		case 3:
			sprintf(text, "%s", "tint");
			change_texbox_text(gui.materialWindow, 7, text);

			sprintf(text, "%s", "fuzz");
			change_texbox_text(gui.materialWindow, 8, text);

			sprintf(text, "(none)");
			change_texbox_text(gui.materialWindow, 9, text);

			sprintf(text, "%01.2f", gui.selectedMaterial.v1);
			change_text_input_text(gui.materialWindow, 3, text);

			sprintf(text, "%01.2f", gui.selectedMaterial.v2);
			change_text_input_text(gui.materialWindow, 4, text);

			sprintf(text, " ");
			change_text_input_text(gui.materialWindow, 5, text);

			break;

		case 4:
			sprintf(text, "%s", "tint");
			change_texbox_text(gui.materialWindow, 7, text);

			sprintf(text, "%s", "fuzz");
			change_texbox_text(gui.materialWindow, 8, text);

			sprintf(text, "%s", "refIdx");
			change_texbox_text(gui.materialWindow, 9, text);

			sprintf(text, "%01.2f", gui.selectedMaterial.v1);
			change_text_input_text(gui.materialWindow, 3, text);

			sprintf(text, "%01.2f", gui.selectedMaterial.v2);
			change_text_input_text(gui.materialWindow, 4, text);

			sprintf(text, "%01.2f", gui.selectedMaterial.v3);
			change_text_input_text(gui.materialWindow, 5, text);

			break;
	}

	change_colored_box_color(gui.materialWindow, 1,
							 (Color){(unsigned char)(gui.selectedMaterial.color.x * 255),
									 (unsigned char)(gui.selectedMaterial.color.y * 255),
									 (unsigned char)(gui.selectedMaterial.color.z * 255), 255});

	return SUCCESS;
}

Status create_ui() {
	gui.infoWindow = add_window(20, 20, 200, 400, "Info");
	add_textbox(gui.infoWindow, 0, 0, "Hello");

	gui.materialWindow = add_window(250, 20, 230, 260, "Material");

	int xOff = 0;
	int yOff = 0;

	add_textbox(gui.materialWindow, 0, yOff, "Color");
	yOff += 20 + 5;

	xOff = 0;

	add_textbox(gui.materialWindow, xOff, yOff, "R:");
	xOff += 20 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", (void *)_r_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_r);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", (void *)_r_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, "G:");
	xOff += 20 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", (void *)_g_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_g);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", (void *)_g_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, "B:");
	xOff += 20 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", (void *)_b_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_b);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", (void *)_b_plus);
	yOff += 20 + 5;

	add_textbox(gui.materialWindow, 0, yOff, "Type");
	yOff += 20 + 5;

	xOff = 0;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "<", (void *)_mat_minus);
	xOff += 20 + 5;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 160 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, ">", (void *)_mat_plus);
	yOff += 20 + 5;

	add_textbox(gui.materialWindow, 0, yOff, "Details");
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 120 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", (void *)_v1_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_v1);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", (void *)_v1_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 120 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", (void *)_v2_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_v2);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", (void *)_v2_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 120 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", (void *)_v3_plus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_v3);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", (void *)_v3_plus);
	yOff += 20 + 5;

	add_colored_box(gui.materialWindow, 140, 30, 60, 60, gui.comp.borderColor);
	add_colored_box(gui.materialWindow, 140 + gui.comp.borderSize, 30 + gui.comp.borderSize,
					60 - gui.comp.borderSize * 2, 60 - gui.comp.borderSize * 2, BLACK);

	return SUCCESS;
}