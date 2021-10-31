#define K_UTIL_IMPLEMENTATION
#include <k_tools/k_util.h>

#define K_OPENCL_UTIL_IMPLEMENTATION
#include <k_tools/k_opencl_util.h>

#define K_IMAGE_IMPLEMENTATION
#include <k_tools/k_image.h>

#include "gui/gui.h"
#include "renderer/renderer.h"
#include "scripting/scripting.h"

int main(int argc, char *argv[]) {
	msg("argc: %d", argc);

	if (create_renderer() != SUCCESS) exit(-1);
	if (init_command_interpreter() != SUCCESS) exit(-1);
	// if (run_script("data/scripts/default.lua", "Main") != SUCCESS) exit(-1);

	set_output_properties(600, 600);

	if (argc < 2) {
		load_scene(NULL);
	} else {
		load_scene(argv[1]);
	}

	create_window();
	start_main_loop();

	close_window();
	close_command_interpreter();
	destroy_renderer();

	return 0;
}