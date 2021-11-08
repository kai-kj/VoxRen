#include "gui.h"

#define AIM_SIZE 5

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

	return SUCCESS;
}