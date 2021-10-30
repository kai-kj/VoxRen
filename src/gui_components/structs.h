#ifndef GUI_COMP_STRUCTS_H
#define GUI_COMP_STRUCTS_H

#include "gui_components.h"

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
	void (*fn)();
	char *text;

	int hover;
	int pressed;
} Button;

typedef struct ColoredBox {
	int x;
	int y;
	int width;
	int height;

	Color color;
} ColoredBox;

typedef struct TextInputBox {
	int x;
	int y;
	int width;
	int height;

	char *text;
	void (*fn)(char *);

	int editing;

} TextInputBox;

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

	int coloredBoxCount;
	ColoredBox *coloredBoxes;

	int textInputBoxCount;
	TextInputBox *textInputBoxes;

} Window;

typedef struct GUI {
	int windowCount;
	Window *windows;

	int fontSize;
	Color fontColor;

	int windowTitleSize;
	Color windowColor;
	Color windowTitleColor;

	int borderSize;
	Color borderColor;

	Color buttonColor;
	Color buttonBorderColor;

	int state;
	int windowIDs;

	int mousePosX;
	int mousePosY;
	int prevMousePosX;
	int prevMousePosY;
} GUI;

#endif