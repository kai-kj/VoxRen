#include "gui_components.h"

GUI *comp;

Status set_comp_address(GUI *ptr) {
	comp = ptr;
	return SUCCESS;
}