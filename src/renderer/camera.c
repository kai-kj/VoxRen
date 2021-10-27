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

RendererStatus set_mouse_pos(int x, int y) {
	r.mousePos.x = x;
	r.mousePos.y = y;

	return RENDERER_SUCCESS;
}

RendererStatus add_voxel_at_mouse(VoxMaterial material) {
	int x = r.lookingAtPos.x + r.lookingAtNormal.x;
	int y = r.lookingAtPos.y + r.lookingAtNormal.y;
	int z = r.lookingAtPos.z + r.lookingAtNormal.z;

	msg("Adding voxel (%d, %d, %d)", x, y, z);
	add_voxel(x, y, z, material);

	return RENDERER_SUCCESS;
}

RendererStatus remove_voxel_at_mouse(VoxMaterial material) {
	if (r.lookingAtPos.x == -1) return RENDERER_FAILURE;

	int x = r.lookingAtPos.x;
	int y = r.lookingAtPos.y;
	int z = r.lookingAtPos.z;

	msg("Removing voxel (%d, %d, %d)", x, y, z);
	remove_voxel(x, y, z);

	return RENDERER_SUCCESS;
}