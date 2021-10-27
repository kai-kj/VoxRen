#include "gui.h"

#define FONT_SIZE 10 * 2
#define AIM_SIZE 5
#define COLOR_STEP 15
#define DETAILS_STEP 0.1

void _r_plus() {
	g.selectedMaterial.color.x += 1.0f / 255.0f * COLOR_STEP;
	if (g.selectedMaterial.color.x > 1) g.selectedMaterial.color.x = 1;
	msg("%f", g.selectedMaterial.color.x);
}

void _r_minus() {
	g.selectedMaterial.color.x -= 1.0f / 255.0f * COLOR_STEP;
	if (g.selectedMaterial.color.x < 0) g.selectedMaterial.color.x = 0;
	msg("%f", g.selectedMaterial.color.x);
}

void _g_plus() {
	g.selectedMaterial.color.y += 1.0f / 255.0f * COLOR_STEP;
	if (g.selectedMaterial.color.y > 1) g.selectedMaterial.color.y = 1;
}

void _g_minus() {
	g.selectedMaterial.color.y -= 1.0f / 255.0f * COLOR_STEP;
	if (g.selectedMaterial.color.y < 0) g.selectedMaterial.color.y = 0;
}

void _b_plus() {
	g.selectedMaterial.color.z += 1.0f / 255.0f * COLOR_STEP;
	if (g.selectedMaterial.color.z > 1) g.selectedMaterial.color.z = 1;
}

void _b_minus() {
	g.selectedMaterial.color.z -= 1.0f / 255.0f * COLOR_STEP;
	if (g.selectedMaterial.color.z < 0) g.selectedMaterial.color.z = 0;
}

void _change_material() {
	switch (g.selectedMaterial.type) {
		case 1:
			g.selectedMaterial.v1 = 2;
			g.selectedMaterial.v2 = 0;
			g.selectedMaterial.v3 = 0;
			break;

		case 2:
			g.selectedMaterial.v1 = 0;
			g.selectedMaterial.v2 = 0;
			g.selectedMaterial.v3 = 0;
			break;

		case 3:
			g.selectedMaterial.v1 = 1;
			g.selectedMaterial.v2 = 0;
			g.selectedMaterial.v3 = 0;
			break;

		case 4:
			g.selectedMaterial.v1 = 1;
			g.selectedMaterial.v2 = 0;
			g.selectedMaterial.v3 = 1;
			break;
	}
}

void _mat_plus() {
	g.selectedMaterial.type++;
	if (g.selectedMaterial.type > 4) g.selectedMaterial.type = 1;
	_change_material();
}

void _mat_minus() {
	g.selectedMaterial.type--;
	if (g.selectedMaterial.type < 1) g.selectedMaterial.type = 4;
	_change_material();
}

void _v1_plus() {
	g.selectedMaterial.v1 += 1.0f * DETAILS_STEP;
	if (g.selectedMaterial.v1 > 1 && g.selectedMaterial.type != 1) g.selectedMaterial.v1 = 1;
}

void _v1_minus() {
	g.selectedMaterial.v1 -= 1.0f * DETAILS_STEP;
	if (g.selectedMaterial.v1 < 0) g.selectedMaterial.v1 = 0;
}

void _v2_plus() {
	g.selectedMaterial.v2 += 1.0f * DETAILS_STEP;
	if (g.selectedMaterial.v2 > 1) g.selectedMaterial.v2 = 1;
}

void _v2_minus() {
	g.selectedMaterial.v2 -= 1.0f * DETAILS_STEP;
	if (g.selectedMaterial.v2 < 0) g.selectedMaterial.v2 = 0;
}

void _v3_plus() {
	g.selectedMaterial.v3 += 1.0f * DETAILS_STEP;
	if (g.selectedMaterial.v3 > 1) g.selectedMaterial.v3 = 1;
}

void _v3_minus() {
	g.selectedMaterial.v3 -= 1.0f * DETAILS_STEP;
	if (g.selectedMaterial.v3 < 0) g.selectedMaterial.v3 = 0;
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
	char text[100];

	sprintf(text, "%03d", (int)(g.selectedMaterial.color.x * 255));
	change_texbox_text(g.materialWindow, 2, text);

	sprintf(text, "%03d", (int)(g.selectedMaterial.color.y * 255));
	change_texbox_text(g.materialWindow, 4, text);

	sprintf(text, "%03d", (int)(g.selectedMaterial.color.z * 255));
	change_texbox_text(g.materialWindow, 6, text);

	switch (g.selectedMaterial.type) {
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

	change_texbox_text(g.materialWindow, 8, text);

	switch (g.selectedMaterial.type) {
		case 1:
			sprintf(text, "%s", "brightness");
			change_texbox_text(g.materialWindow, 10, text);

			sprintf(text, "(none)");
			change_texbox_text(g.materialWindow, 12, text);

			sprintf(text, "(none)");
			change_texbox_text(g.materialWindow, 14, text);

			sprintf(text, "%01.2f", g.selectedMaterial.v1);
			change_texbox_text(g.materialWindow, 11, text);

			sprintf(text, "");
			change_texbox_text(g.materialWindow, 13, text);

			sprintf(text, "");
			change_texbox_text(g.materialWindow, 15, text);

			break;

		case 2:
			sprintf(text, "(none)");
			change_texbox_text(g.materialWindow, 10, text);

			sprintf(text, "(none)");
			change_texbox_text(g.materialWindow, 12, text);

			sprintf(text, "(none)");
			change_texbox_text(g.materialWindow, 14, text);

			sprintf(text, "");
			change_texbox_text(g.materialWindow, 11, text);

			sprintf(text, "");
			change_texbox_text(g.materialWindow, 13, text);

			sprintf(text, "");
			change_texbox_text(g.materialWindow, 15, text);

			break;

		case 3:
			sprintf(text, "%s", "tint");
			change_texbox_text(g.materialWindow, 10, text);

			sprintf(text, "%s", "fuzz");
			change_texbox_text(g.materialWindow, 12, text);

			sprintf(text, "(none)");
			change_texbox_text(g.materialWindow, 14, text);

			sprintf(text, "%01.2f", g.selectedMaterial.v1);
			change_texbox_text(g.materialWindow, 11, text);

			sprintf(text, "%01.2f", g.selectedMaterial.v2);
			change_texbox_text(g.materialWindow, 13, text);

			sprintf(text, "");
			change_texbox_text(g.materialWindow, 15, text);

			break;

		case 4:
			sprintf(text, "%s", "tint");
			change_texbox_text(g.materialWindow, 10, text);

			sprintf(text, "%s", "fuzz");
			change_texbox_text(g.materialWindow, 12, text);

			sprintf(text, "%s", "refIdx");
			change_texbox_text(g.materialWindow, 14, text);

			sprintf(text, "%01.2f", g.selectedMaterial.v1);
			change_texbox_text(g.materialWindow, 11, text);

			sprintf(text, "%01.2f", g.selectedMaterial.v2);
			change_texbox_text(g.materialWindow, 13, text);

			sprintf(text, "%01.2f", g.selectedMaterial.v3);
			change_texbox_text(g.materialWindow, 15, text);

			break;
	}

	change_colored_box_color(g.materialWindow, 1,
							 (Color){(unsigned char)(g.selectedMaterial.color.x * 255),
									 (unsigned char)(g.selectedMaterial.color.y * 255),
									 (unsigned char)(g.selectedMaterial.color.z * 255), 255});

	return GUI_SUCCESS;
}

GUIStatus create_ui() {
	g.infoWindow = add_window(20, 20, 200, 400, "Info");
	add_textbox(g.infoWindow, 0, 0, "Hello");

	g.materialWindow = add_window(250, 20, 230, 260, "Material");

	int xOff = 0;
	int yOff = 0;

	add_textbox(g.materialWindow, 0, yOff, "Color");
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

	add_textbox(g.materialWindow, 0, yOff, "Type");
	yOff += 20 + 5;

	xOff = 0;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "<", (void *)_mat_minus);
	xOff += 20 + 5;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 160 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, ">", (void *)_mat_plus);
	yOff += 20 + 5;

	add_textbox(g.materialWindow, 0, yOff, "Details");
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 120 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "-", (void *)_v1_minus);
	xOff += 20 + 5;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 40 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "+", (void *)_v1_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 120 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "-", (void *)_v2_minus);
	xOff += 20 + 5;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 40 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "+", (void *)_v2_plus);
	yOff += 20 + 5;

	xOff = 0;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 120 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "-", (void *)_v3_plus);
	xOff += 20 + 5;
	add_textbox(g.materialWindow, xOff, yOff, "");
	xOff += 40 + 5;
	add_button(g.materialWindow, xOff, yOff, 20, 20, "+", (void *)_v3_plus);
	yOff += 20 + 5;

	add_colored_box(g.materialWindow, 140, 30, 60, 60, g.settings.borderColor);
	add_colored_box(g.materialWindow, 140 + g.settings.borderSize, 30 + g.settings.borderSize,
					60 - g.settings.borderSize * 2, 60 - g.settings.borderSize * 2, BLACK);

	return GUI_SUCCESS;
}