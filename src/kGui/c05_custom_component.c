#include "kGui.h"

Component create_custom_component(void (*fn)(int, int)) {
	Component component;
	component.type = 5;
	component.data.customComponent.fn = fn;
	return component;
}

void _draw_custom_component(CustomComponent *customComponent, int x, int y) {
	customComponent->fn(x, y);
}