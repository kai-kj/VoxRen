#include "gui.h"

#define FONT_SIZE 10 * 2
#define AIM_SIZE 5
#define STEP 5

void _r_plus() {
	g.selectedMaterial.color.x += 1.0f / 255.0f * STEP;
	if (g.selectedMaterial.color.x > 1) g.selectedMaterial.color.x = 1;
	msg("%f", g.selectedMaterial.color.x);
}

void _r_minus() {
	g.selectedMaterial.color.x -= 1.0f / 255.0f * STEP;
	if (g.selectedMaterial.color.x < 0) g.selectedMaterial.color.x = 0;
	msg("%f", g.selectedMaterial.color.x);
}

void _g_plus() {
	g.selectedMaterial.color.y += 1.0f / 255.0f * STEP;
	if (g.selectedMaterial.color.y > 1) g.selectedMaterial.color.y = 1;
}

void _g_minus() {
	g.selectedMaterial.color.y -= 1.0f / 255.0f * STEP;
	if (g.selectedMaterial.color.y < 0) g.selectedMaterial.color.y = 0;
}

void _b_plus() {
	g.selectedMaterial.color.z += 1.0f / 255.0f * STEP;
	if (g.selectedMaterial.color.z > 1) g.selectedMaterial.color.z = 1;
}

void _b_minus() {
	g.selectedMaterial.color.z -= 1.0f / 255.0f * STEP;
	if (g.selectedMaterial.color.z < 0) g.selectedMaterial.color.z = 0;
}

GUIStatus draw_aim() {
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE, BLACK);
	DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, AIM_SIZE / 2, WHITE);

	return GUI_SUCCESS;
}

GUIStatus update_info_window() {
	char text[1000];
	sprintf(text, "size %dx%d\nfps %03d\nvoxels %d\nchunks %d", r.image.size.x, r.image.size.y, (int)(1.0 / r.dt),
			r.scene.voxelCount, r.scene.chunkCount);
	change_texbox_text(g.infoWindow, 0, text);

	return GUI_SUCCESS;
}

GUIStatus update_material_window() {
	char red[5] = "";
	sprintf(red, "%03d", (int)(g.selectedMaterial.color.x * 255));
	change_texbox_text(g.materialWindow, 2, red);

	char green[5] = "";
	sprintf(green, "%03d", (int)(g.selectedMaterial.color.y * 255));
	change_texbox_text(g.materialWindow, 4, green);

	char blue[5] = "";
	sprintf(blue, "%03d", (int)(g.selectedMaterial.color.z * 255));
	change_texbox_text(g.materialWindow, 6, blue);

	return GUI_SUCCESS;
}

GUIStatus create_ui() {
	g.infoWindow = add_window(20, 20, 200, 400, "Info");
	add_textbox(g.infoWindow, 0, 0, "Hello");

	g.materialWindow = add_window(600, 20, 600, 400, "Material");

	int xOff = 0;
	int yOff = 0;

	add_textbox(g.materialWindow, 0, 0, "Color");
	yOff += 20 + 5;

	xOff = 0;

	add_textbox(g.materialWindow, xOff, yOff, "R:");
	xOff += 20 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "-", (void *)_r_minus);
	xOff += 20 + 5;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 40 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "+", (void *)_r_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(g.materialWindow, xOff, yOff, "G:");
	xOff += 20 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "-", (void *)_g_minus);
	xOff += 20 + 5;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 40 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "+", (void *)_g_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(g.materialWindow, xOff, yOff, "B:");
	xOff += 20 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "-", (void *)_b_minus);
	xOff += 20 + 5;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 40 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "+", (void *)_b_plus);
	yOff += 20 + 5;

	return GUI_SUCCESS;
}