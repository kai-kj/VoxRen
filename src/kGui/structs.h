typedef int WindowId;
typedef int ComponentID;

typedef struct Component Component;
typedef struct Grid {
	int cols;
	int rows;

	int cellWidth;
	int cellHeight;

	ComponentID *components;
} Grid;

typedef struct Label {
	char *text;
} Label;

typedef struct Button {
	char *text;
	int width;
	int height;
	void (*fn)();
	int pressed;
} Button;

typedef struct Textbox {
	char *text;
	int onlyDigits;
	int width;
	int height;
	void (*fn)();
	int editing;
	int mouseInBounds;
} Textbox;

typedef struct PositionTracker {
	int x;
	int y;
} PositionTracker;

typedef struct Component {
	int type;
	union {
		Grid grid;
		Label label;
		Button button;
		Textbox textbox;
		PositionTracker posTracker;
	} data;
} Component;

typedef struct Window {
	int x, y;
	char *title;

	int dragging;

	ComponentID grid;
} Window;

typedef struct Settings {
	int padding;

	int fontSize;
	Color fontColor;

	Color windowColor;

	int titleBarHeight;
	Color titleBarColor;

	int borderSize;
	Color borderColor;

	int buttonBorderSize;
	Color buttonReleasedColor;
	Color buttonPressedColor;

	int textboxBorderSize;
	Color textboxReleasedColor;
	Color textboxPressedColor;

} Settings;

typedef struct KGuiState {
	int windowCount;
	Window *windows;

	int componentCount;
	Component *components;

	Settings settings;

	int prevMouseX;
	int prevMouseY;
} KGuiState;