#include "../interface.h"

void _set_x(char *text) {
	ren.camera.pos.x = atof(text);
	ren.restartRender = 1;
}

void _set_y(char *text) {
	ren.camera.pos.y = atof(text);
	ren.restartRender = 1;
}

void _set_z(char *text) {
	ren.camera.pos.z = atof(text);
	ren.restartRender = 1;
}

void _set_yaw(char *text) {
	set_camera_pos(ren.camera.pos.x, ren.camera.pos.y, ren.camera.pos.z, atof(text), ren.camera.rot.y);
	ren.restartRender = 1;
}

void _set_pitch(char *text) {
	set_camera_pos(ren.camera.pos.x, ren.camera.pos.y, ren.camera.pos.z, ren.camera.rot.x, atof(text));
	ren.restartRender = 1;
}

void _set_sensor_width(char *text) {
	ren.camera.sensorWidth = atof(text);
	ren.restartRender = 1;
}

void _set_focal_length(char *text) {
	ren.camera.focalLength = atof(text);
	ren.restartRender = 1;
}

void _set_aperture(char *text) {
	ren.camera.aperture = atof(text);
	ren.restartRender = 1;
}

void _set_exposure(char *text) {
	ren.camera.exposure = atof(text);
	ren.restartRender = 1;
}

void _set_look_sensitivity(char *text) {
	gui.cameraLookSpeed = atof(text);
}

void _set_move_sensitivity(char *text) {
	gui.cameraMoveSpeed = atof(text);
}

void _set_bg_r(char *text) {
	float color = (float)atoi(text) / 255.0;
	ren.scene.bgColor.x = color > 1 ? 1 : color < 0 ? 0 : color;
	ren.restartRender = 1;
}

void _set_bg_g(char *text) {
	float color = (float)atoi(text) / 255.0;
	ren.scene.bgColor.y = color > 1 ? 1 : color < 0 ? 0 : color;
	ren.restartRender = 1;
}

void _set_bg_b(char *text) {
	float color = (float)atoi(text) / 255.0;
	ren.scene.bgColor.z = color > 1 ? 1 : color < 0 ? 0 : color;
	ren.restartRender = 1;
}

void _set_bg_brightness(char *text) {
	ren.scene.bgBrightness = atof(text);
	ren.restartRender = 1;
}

void _create_scene_window() {
	ComponentID w = create_window(GetScreenWidth() - 250 - 40, 20, 1, 18, 250, 40, "Scene");
	ComponentID g;

	add_component(w, 0, 0, create_label("Camera"));

	g = add_component(w, 0, 1, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  x:"));
	gui.components.cameraX = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_x));

	g = add_component(w, 0, 2, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  y:"));
	gui.components.cameraY = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_y));

	g = add_component(w, 0, 3, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  z:"));
	gui.components.cameraZ = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_z));

	g = add_component(w, 0, 4, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  yaw:"));
	gui.components.cameraYaw = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_yaw));

	g = add_component(w, 0, 5, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  pitch:"));
	gui.components.cameraPitch = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_pitch));

	g = add_component(w, 0, 6, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  sensor width:"));
	gui.components.cameraSensorWidth = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_sensor_width));

	g = add_component(w, 0, 7, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  focal length:"));
	gui.components.cameraFocalLength = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_focal_length));

	g = add_component(w, 0, 8, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  aperture:"));
	gui.components.cameraAperture = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_aperture));

	g = add_component(w, 0, 9, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  exposure:"));
	gui.components.cameraExposure = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_exposure));

	add_component(w, 0, 10, create_label("Sensitivity"));

	g = add_component(w, 0, 11, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  Look:"));
	gui.components.lookSensitivity = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_look_sensitivity));

	g = add_component(w, 0, 12, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  Move:"));
	gui.components.moveSensitivity = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_move_sensitivity));

	add_component(w, 0, 13, create_label("Background"));

	g = add_component(w, 0, 14, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  R:"));
	gui.components.bgRed = add_component(g, 1, 0, create_textbox(70, 25, "000", _set_bg_r));

	g = add_component(w, 0, 15, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  G:"));
	gui.components.bgGreen = add_component(g, 1, 0, create_textbox(70, 25, "000", _set_bg_g));

	g = add_component(w, 0, 16, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  B:"));
	gui.components.bgBlue = add_component(g, 1, 0, create_textbox(70, 25, "000", _set_bg_b));

	g = add_component(w, 0, 17, create_grid(2, 1, 160, 20));
	add_component(g, 0, 0, create_label("  Brightness:"));
	gui.components.bgBrightness = add_component(g, 1, 0, create_textbox(70, 25, "0.0", _set_bg_brightness));
}

void _update_scene_window() {
	float pitch = ren.camera.rot.x * 180 / PI;
	float yaw = ren.camera.rot.y * 180 / PI;

	change_component_text(gui.components.cameraX, "%.01f", ren.camera.pos.x);
	change_component_text(gui.components.cameraY, "%.01f", ren.camera.pos.y);
	change_component_text(gui.components.cameraZ, "%.01f", ren.camera.pos.z);
	change_component_text(gui.components.cameraPitch, "%.01f", yaw);
	change_component_text(gui.components.cameraYaw, "%.01f", pitch);
	change_component_text(gui.components.cameraSensorWidth, "%.01f", ren.camera.sensorWidth);
	change_component_text(gui.components.cameraFocalLength, "%.01f", ren.camera.focalLength);
	change_component_text(gui.components.cameraAperture, "%.04f", ren.camera.aperture);
	change_component_text(gui.components.cameraExposure, "%.01f", ren.camera.exposure);
	change_component_text(gui.components.lookSensitivity, "%.01f", gui.cameraLookSpeed);
	change_component_text(gui.components.moveSensitivity, "%.01f", gui.cameraMoveSpeed);
	change_component_text(gui.components.bgRed, "%03d", (int)(ren.scene.bgColor.x * 255));
	change_component_text(gui.components.bgGreen, "%03d", (int)(ren.scene.bgColor.y * 255));
	change_component_text(gui.components.bgBlue, "%03d", (int)(ren.scene.bgColor.z * 255));
	change_component_text(gui.components.bgBrightness, "%.01f", ren.scene.bgBrightness);
}