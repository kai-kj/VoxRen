#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include "gui.h"

typedef struct TextBox {
	int x;
	int y;
	char *text;

} TextBox;

typedef struct Button {
	int x;
	int y;
	int width;
	int height;
	void *(*fn)();
	char *text;

	int hover;
	int pressed;
} Button;

typedef struct Window {
	int id;

	int x;
	int y;
	int width;
	int height;
	char *title;

	int dragging;

	int textBoxCount;
	TextBox *textBoxes;

	int buttonCount;
	Button *buttons;

} Window;

typedef struct Settings {
	int fontSize;
	Color fontColor;

	int windowTitleSize;
	Color windowColor;
	Color windowTitleColor;

	int borderSize;
	Color borderColor;

	Color buttonColor;
	Color buttonBorderColor;
} Settings;

typedef struct GUIState {
	Texture2D renderTexture;

	int width;
	int height;

	double prevTime;

	int mousePosX;
	int mousePosY;
	int prevMousePosX;
	int prevMousePosY;

	int firstChar;
	int commandLength;
	char *command;

	int quit;
	int state;

	int windowIDs;
	int windowCount;
	Window *windows;

	int infoWindow;
	int materialWindow;

	VoxMaterial selectedMaterial;

	Settings settings;
} GUIState;

#endif