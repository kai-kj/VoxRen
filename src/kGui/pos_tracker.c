#include "kGui.h"

Component create_pos_tracker() {
	Component component;
	component.type = 4;
	return component;
}

int get_pos_tracker_x(ComponentID id) {
	return _get_component(id)->data.posTracker.x;
}

int get_pos_tracker_y(ComponentID id) {
	return _get_component(id)->data.posTracker.y;
}

void _process_pos_tracker(PositionTracker *tracker, int x, int y) {
	tracker->x = x;
	tracker->y = y;
}