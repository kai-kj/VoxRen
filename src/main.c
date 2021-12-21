#define K_UTIL_IMPLEMENTATION
#include <k_tools/k_util.h>

#define K_OPENCL_UTIL_IMPLEMENTATION
#include <k_tools/k_opencl_util.h>

// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "headless/headless.h"
#include "interface/interface.h"
#include "renderer/renderer.h"

/*
- gui mode
  VoxRen g [file name] [width] [height]
- headless mode
  VoxRen h [file name] [width] [height] [render samples] [output file]

- resolutions:
  - 4k: 3840 2160
*/

void load_scene(int argc, char **argv) {
	if (argc < 2) panic("Not enough arguments");

	if (strcmp(argv[1], "g") == 0) {
		if (argc != 5) panic("Not enough arguments");

		int width = atoi(argv[3]);
		int height = atoi(argv[4]);

		set_output_properties(width, height);
		load_scene_from_file(argv[2]);
		initialise_interface();
		start_main_loop();
		destroy_renderer();

	} else if (strcmp(argv[1], "h") == 0) {
		if (argc != 7) panic("Invalid arguments");

		int width = atoi(argv[3]);
		int height = atoi(argv[4]);
		int samples = atoi(argv[5]);

		load_scene_from_file(argv[2]);
		set_output_properties(width, height);
		render_image(samples, argv[6]);
	}
}

int main(int argc, char **argv) {
	if (create_renderer() != SUCCESS) exit(-1);
	load_scene(argc, argv);

	return 0;
}