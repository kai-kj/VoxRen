#include "renderer.h"

RendererStatus set_camera_properties(float sensorWidth, float focalLength, float aperture, float exposure) {
	r.camera.sensorWidth = sensorWidth;
	r.camera.focalLength = focalLength;
	r.camera.aperture = aperture;
	r.camera.exposure = exposure;

	return RENDERER_SUCCESS;
}

RendererStatus set_camera_pos(float x, float y, float z, float rx, float ry) {
	r.camera.pos = (cl_float3){.x = x, .y = y, .z = z};
	r.camera.rot = (cl_float2){.x = rx, .y = ry};

	return RENDERER_SUCCESS;
}