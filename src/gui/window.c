#include "gui.h"

//---- private ---------------------------------------------------------------//

//---- public ----------------------------------------------------------------//

GUIStatus create_window(int width, int height) {
	msg("Creating window");
	InitWindow(width, height, "VoxRen");
	SetTargetFPS(30);

	Image tmpImg = GenImageColor(width, height, BLACK);
	g.renderTexture = LoadTextureFromImage(tmpImg);
	UnloadImage(tmpImg);

	g.commandLength = 1;
	g.command = '\0';

	g.draging = 0;
	g.commandMode = 0;

	return GUI_SUCCESS;
}

GUIStatus close_window() {
	msg("Closing window");
	CloseWindow();

	return GUI_SUCCESS;
}