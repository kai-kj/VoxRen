#include <CL/cl.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>

#define PI 3.14159265358979323846

#define K_UTIL_IMPLEMENTATION
#include <k_tools/k_util.h>

#define K_OPENCL_UTIL_IMPLEMENTATION
#include <k_tools/k_opencl_util.h>

#define K_IMAGE_IMPLEMENTATION
#include <k_tools/k_image.h>

// #define K_OPENCL_IMPLEMENTATION
// #include "k_tools/k_opencl.h"

#include "gui/gui.h"
#include "renderer/renderer.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(void) {
	if (create_renderer() != RENDERER_SUCCESS)
		exit(-1);

	set_output_properties(800, 600);

	int size = 10;

	int width = size;
	int height = size;
	int depth = size;

	set_background_properties(0.5, 0.5, 1, 0.1);

	Material white = create_lambertian_material(0.8, 0.8, 0.8);
	Material red = create_lambertian_material(1, 0, 0);
	Material green = create_lambertian_material(0, 1, 0);
	Material blue = create_lambertian_material(0, 0, 1);
	Material mirror = create_metal_material(0, 0, 0, 0.5, 0);
	Material light = create_light_source_material(1, 1, 0.5, 2);

	// side walls
	for (int y = 0; y < width; ++y) {
		for (int z = 0; z < depth; ++z) {
			add_voxel(0, y, z, red);
			add_voxel(width - 1, y, z, green);
		}
	}

	// back wall
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			add_voxel(x, y, depth - 1, blue);
		}
	}

	// celling
	for (int x = 0; x < width; x++) {
		for (int z = 0; z < depth; z++) {
			add_voxel(x, 0, z, white);
		}
	}

	// floor
	for (int x = 0; x < width; x++) {
		for (int z = 0; z < depth; z++) {
			add_voxel(x, height - 1, z, white);
		}
	}

	// test chunk add/removal
	add_voxel(2 * width, 0, 0, white);
	remove_voxel(2 * width, 0, 0);

	// objects
	add_voxel(width * 0.7, height - 1, depth * 0.5, light);
	add_voxel(width * 0.7, height - 2, depth * 0.5, light);

	add_voxel(width * 0.3, height - 1, depth * 0.3, white);
	add_voxel(width * 0.3, height - 2, depth * 0.3, white);

	set_camera_properties(5.5, -2, -10, -M_PI / 8, 0, 0, 1, 1, 0.001, 1000);

	if (create_window(SCREEN_WIDTH, SCREEN_HEIGHT) == GUI_SUCCESS)
		start_main_loop();

	close_window();

	// render_image_to_file(samples, "render.png");

	destroy_renderer();

	return 0;
}