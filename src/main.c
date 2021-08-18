#define K_UTIL_IMPLEMENTATION
#include <k_tools/k_util.h>

#define K_OPENCL_UTIL_IMPLEMENTATION
#include <k_tools/k_opencl_util.h>

#define K_IMAGE_IMPLEMENTATION
#include <k_tools/k_image.h>

#include "gui/gui.h"
#include "renderer/renderer.h"
#include "scripting/scripting.h"

#define PI 3.14159265358979323846

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(void) {
	if (create_renderer() != RENDERER_SUCCESS)
		exit(-1);

	if (run_script("main.lua", "Main") != SCRIPTING_SUCCESS)
		exit(-1);

	if (create_window(SCREEN_WIDTH, SCREEN_HEIGHT) == GUI_SUCCESS)
		start_main_loop();

	close_window();

	// render_image_to_file(samples, "render.png");

	destroy_renderer();

	return 0;
}