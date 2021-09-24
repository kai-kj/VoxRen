#ifndef RENDERER_H
#define RENDERER_H

#include <k_tools/k_image.h>
#include <k_tools/k_opencl_util.h>
#include <k_tools/k_util.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "structs.h"

typedef char RendererStatus;
#define RENDERER_SUCCESS 0
#define RENDERER_FAILURE -1

extern Renderer r;

//---- management ------------------------------------------------------------//

RendererStatus create_renderer();
RendererStatus destroy_renderer();
RendererStatus begin_rendering();
RendererStatus end_rendering();
k_Image *get_image();

//---- scene -----------------------------------------------------------------//

RendererStatus set_output_properties(int width, int height);
RendererStatus set_background_properties(float red, float green, float blue,
										 float brightness);
RendererStatus add_voxel(int x, int y, int z, VoxMaterial material);
RendererStatus remove_voxel(int x, int y, int z);

//---- camera ----------------------------------------------------------------//

RendererStatus set_camera_properties(float sensorWidth, float focalLength,
									 float aperture, float exposure);
RendererStatus set_camera_pos(float x, float y, float z, float rx, float ry);

//---- material --------------------------------------------------------------//

VoxMaterial create_light_source_material(float r, float g, float b,
										 float brightness);
VoxMaterial create_lambertian_material(float r, float g, float b);
VoxMaterial create_metal_material(float r, float g, float b, float tint,
								  float fuzz);
VoxMaterial create_dielectric_material(float r, float g, float b, float tint,
									   float fuzz, float refIdx);

#endif
