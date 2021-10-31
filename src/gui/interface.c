#include "gui.h"

#define FONT_SIZE 10 * 2
#define AIM_SIZE 5
#define COLOR_STEP 15
#define DETAILS_STEP 0.1

void _r_plus() {
	gui.selectedMaterial.color.x += 1.0f / 255.0f * COLOR_STEP;
	if (gui.selectedMaterial.color.x > 1) gui.selectedMaterial.color.x = 1;
}

void _r_minus() {
	msg("r-");
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

void _bg_r_plus() {
	ren.scene.bgColor.x += 1.0f / 255.0f * COLOR_STEP;
	if (ren.scene.bgColor.x > 1) ren.scene.bgColor.x = 1;
	ren.restartRender = 1;
}

void _bg_r_minus() {
	ren.scene.bgColor.x -= 1.0f / 255.0f * COLOR_STEP;
	if (ren.scene.bgColor.x < 0) ren.scene.bgColor.x = 0;
	ren.restartRender = 1;
}

void _bg_g_plus() {
	ren.scene.bgColor.y += 1.0f / 255.0f * COLOR_STEP;
	if (ren.scene.bgColor.y > 1) ren.scene.bgColor.y = 1;
	ren.restartRender = 1;
}

void _bg_g_minus() {
	ren.scene.bgColor.y -= 1.0f / 255.0f * COLOR_STEP;
	if (ren.scene.bgColor.y < 0) ren.scene.bgColor.y = 0;
	ren.restartRender = 1;
}

void _bg_b_plus() {
	ren.scene.bgColor.z += 1.0f / 255.0f * COLOR_STEP;
	if (ren.scene.bgColor.z > 1) ren.scene.bgColor.z = 1;
	ren.restartRender = 1;
}

void _bg_b_minus() {
	ren.scene.bgColor.z -= 1.0f / 255.0f * COLOR_STEP;
	if (ren.scene.bgColor.z < 0) ren.scene.bgColor.z = 0;
	ren.restartRender = 1;
}

void _set_bg_r(char *text) {
	float color = (float)atoi(text) / 255.0;
	if (color > 1) color = 1;
	if (color < 0) color = 0;

	ren.scene.bgColor.x = color;
	ren.restartRender = 1;
}

void _set_bg_g(char *text) {
	float color = (float)atoi(text) / 255.0;
	if (color > 1) color = 1;
	if (color < 0) color = 0;

	ren.scene.bgColor.y = color;
	ren.restartRender = 1;
}

void _set_bg_b(char *text) {
	float color = (float)atoi(text) / 255.0;
	if (color > 1) color = 1;
	if (color < 0) color = 0;

	ren.scene.bgColor.z = color;
	ren.restartRender = 1;
}

void _set_bg_br(char *text) {
	float brightness = (float)atoi(text);
	if (brightness < 0) brightness = 0;

	ren.scene.bgBrightness = brightness;
	ren.restartRender = 1;
}

void _save_file() {
	save_scene(ren.fileName);
}

void _save_new_file() {
	char newFileName[1000];
	sprintf(newFileName, "%s%d", ren.fileName, (int)time(NULL));
	save_scene(newFileName);
}

void _set_move_speed(char *text) {
	float speed = (float)atoi(text);
	if (speed < 0) speed = 0;

	gui.cameraMoveSpeed = speed;
}

void _set_look_speed(char *text) {
	float speed = (float)atoi(text);
	if (speed < 0) speed = 0;

	gui.cameraLookSpeed = speed;
}

Status draw_aim() {
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE, BLACK);
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE / 2, WHITE);

	return SUCCESS;
}

static void _update_info_window() {
	char text[100];

	sprintf(text, "%d fps", GetFPS());
	change_texbox_text(gui.infoWindow, 1, text);

	sprintf(text, "%d rps", (int)(1.0f / ren.dt));
	change_texbox_text(gui.infoWindow, 2, text);

	sprintf(text, "%d voxels", ren.scene.voxelCount);
	change_texbox_text(gui.infoWindow, 4, text);

	sprintf(text, "%d chunks", ren.scene.chunkCount);
	change_texbox_text(gui.infoWindow, 5, text);

	sprintf(text, "pixel (%d, %d)", ren.mousePos.x, ren.mousePos.y);
	change_texbox_text(gui.infoWindow, 7, text);

	if (ren.lookingAt)
		sprintf(text, "voxel (%d, %d, %d)", ren.lookingAtPos.x, ren.lookingAtPos.y, ren.lookingAtPos.z);
	else
		sprintf(text, "voxel (none)");
	change_texbox_text(gui.infoWindow, 8, text);
}

static void _update_material_window() {
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
}

static void _update_file_window() {
	change_texbox_text(gui.fileWindow, 0, ren.fileName);
}

static void _update_camera_window() {
	char text[100];

	sprintf(text, "%03d", (int)(ren.scene.bgColor.x * 255));
	change_text_input_text(gui.cameraWindow, 0, text);

	sprintf(text, "%03d", (int)(ren.scene.bgColor.y * 255));
	change_text_input_text(gui.cameraWindow, 1, text);

	sprintf(text, "%03d", (int)(ren.scene.bgColor.z * 255));
	change_text_input_text(gui.cameraWindow, 2, text);

	sprintf(text, "%0.2f", ren.scene.bgBrightness);
	change_text_input_text(gui.cameraWindow, 3, text);

	sprintf(text, "%0.2f", gui.cameraMoveSpeed);
	change_text_input_text(gui.cameraWindow, 4, text);

	sprintf(text, "%0.2f", gui.cameraLookSpeed);
	change_text_input_text(gui.cameraWindow, 5, text);
}

Status update_windows() {
	_update_info_window();
	_update_material_window();
	_update_file_window();
	_update_camera_window();
}

Status create_ui() {

	int xOff = 0;
	int yOff = 0;

	gui.infoWindow = add_window(20, 20, 200, 240, "Info");

	xOff = 20;
	yOff = 0;

	add_textbox(gui.infoWindow, 0, yOff, "Performance");
	yOff += 20 + 5;
	add_textbox(gui.infoWindow, xOff, yOff, " ");
	yOff += 20 + 5;
	add_textbox(gui.infoWindow, xOff, yOff, " ");
	yOff += 20 + 5;

	add_textbox(gui.infoWindow, 0, yOff, "Scene");
	yOff += 20 + 5;
	add_textbox(gui.infoWindow, xOff, yOff, " ");
	yOff += 20 + 5;
	add_textbox(gui.infoWindow, xOff, yOff, " ");
	yOff += 20 + 5;

	add_textbox(gui.infoWindow, 0, yOff, "Looking at");
	yOff += 20 + 5;
	add_textbox(gui.infoWindow, xOff, yOff, " ");
	yOff += 20 + 5;
	add_textbox(gui.infoWindow, xOff, yOff, " ");
	yOff += 20 + 5;

	// material selector

	gui.materialWindow = add_window(250, 20, 230, 260, "Material");

	xOff = 0;
	yOff = 0;

	add_textbox(gui.materialWindow, 0, yOff, "Color");
	yOff += 20 + 5;

	xOff = 0;

	add_textbox(gui.materialWindow, xOff, yOff, "R:");
	xOff += 20 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", _r_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_r);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", _r_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, "G:");
	xOff += 20 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", _g_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_g);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", _g_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, "B:");
	xOff += 20 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", _b_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_b);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", _b_plus);
	yOff += 20 + 5;

	add_textbox(gui.materialWindow, 0, yOff, "Type");
	yOff += 20 + 5;

	xOff = 0;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "<", _mat_minus);
	xOff += 20 + 5;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 160 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, ">", _mat_plus);
	yOff += 20 + 5;

	add_textbox(gui.materialWindow, 0, yOff, "Details");
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 120 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", _v1_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_v1);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", _v1_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 120 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", _v2_minus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_v2);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", _v2_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.materialWindow, xOff, yOff, " ");
	xOff += 120 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "-", _v3_plus);
	xOff += 20 + 5;
	add_text_input(gui.materialWindow, xOff, yOff, 40, 20, " ", _set_v3);
	xOff += 40 + 5;
	add_button(gui.materialWindow, xOff, yOff, 20, 20, "+", _v3_plus);
	yOff += 20 + 5;

	add_colored_box(gui.materialWindow, 140, 30, 60, 60, gui.comp.borderColor);
	add_colored_box(gui.materialWindow, 140 + gui.comp.borderSize, 30 + gui.comp.borderSize,
					60 - gui.comp.borderSize * 2, 60 - gui.comp.borderSize * 2, BLACK);

	// file

	gui.fileWindow = add_window(20, 320, 300, 60, "File");

	xOff = 0;
	yOff = 0;

	add_textbox(gui.fileWindow, 0, 0, " ");
	add_button(gui.fileWindow, xOff, 25, 60, 20, "Save", _save_file);
	xOff += 60 + 5;
	add_button(gui.fileWindow, xOff, 25, 140, 20, "Save as new", _save_new_file);

	// camera

	gui.cameraWindow = add_window(20, 420, 200, 210, "Camera");

	xOff = 0;
	yOff = 0;

	add_textbox(gui.cameraWindow, 0, yOff, "Background color");
	yOff += 20 + 5;

	xOff = 0;

	add_textbox(gui.cameraWindow, xOff, yOff, "R:");
	xOff += 20 + 5;
	add_button(gui.cameraWindow, xOff, yOff, 20, 20, "-", _bg_r_minus);
	xOff += 20 + 5;
	add_text_input(gui.cameraWindow, xOff, yOff, 40, 20, " ", _set_bg_r);
	xOff += 40 + 5;
	add_button(gui.cameraWindow, xOff, yOff, 20, 20, "+", _bg_r_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.cameraWindow, xOff, yOff, "G:");
	xOff += 20 + 5;
	add_button(gui.cameraWindow, xOff, yOff, 20, 20, "-", _bg_g_minus);
	xOff += 20 + 5;
	add_text_input(gui.cameraWindow, xOff, yOff, 40, 20, " ", _set_bg_g);
	xOff += 40 + 5;
	add_button(gui.cameraWindow, xOff, yOff, 20, 20, "+", _bg_g_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.cameraWindow, xOff, yOff, "B:");
	xOff += 20 + 5;
	add_button(gui.cameraWindow, xOff, yOff, 20, 20, "-", _bg_b_minus);
	xOff += 20 + 5;
	add_text_input(gui.cameraWindow, xOff, yOff, 40, 20, " ", _set_bg_b);
	xOff += 40 + 5;
	add_button(gui.cameraWindow, xOff, yOff, 20, 20, "+", _bg_b_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(gui.cameraWindow, xOff, yOff, "Brightness:");
	xOff += 120 + 5;
	add_text_input(gui.cameraWindow, xOff, yOff, 60, 20, " ", _set_bg_br);
	yOff += 20 + 5;

	add_textbox(gui.cameraWindow, 0, yOff, "Camera sensitivity");
	yOff += 20 + 5;
	add_textbox(gui.cameraWindow, 0, yOff, "Move");
	add_text_input(gui.cameraWindow, 65, yOff, 60, 20, " ", _set_move_speed);
	yOff += 20 + 5;
	add_textbox(gui.cameraWindow, 0, yOff, "Look");
	add_text_input(gui.cameraWindow, 65, yOff, 60, 20, " ", _set_look_speed);
	yOff += 20 + 5;

	return SUCCESS;
}