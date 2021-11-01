#include "gui_components.h"

Status update_gui_components() {
	if (comp->state == 0) return FAILURE;

	for (int i = 0; i < comp->windowCount; i++) {
		update_window_pos(&comp->windows[i]);
		process_buttons(&comp->windows[i]);
		procces_text_inputs(&comp->windows[i]);
	}

	return SUCCESS;
}

Status draw_gui_components() {
	for (int i = 0; i < comp->windowCount; i++) {
		Window w = comp->windows[i];
		draw_window(w);
		draw_colored_boxes(w);
		draw_buttons(w);
		draw_textboxes(w);
		draw_text_inputs(w);
	}

	return SUCCESS;
}