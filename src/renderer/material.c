#include "renderer.h"

VoxMaterial create_light_source_material(float r, float g, float b, float brightness) {
	VoxMaterial m;
	m.type = 1;
	m.color = (cl_float3){r, g, b};
	m.v1 = brightness;
	m.v2 = 0;
	m.v3 = 0;

	return m;
}

VoxMaterial create_lambertian_material(float r, float g, float b) {
	VoxMaterial m;
	m.type = 2;
	m.color = (cl_float3){r, g, b};
	m.v1 = 0;
	m.v2 = 0;
	m.v3 = 0;

	return m;
}

VoxMaterial create_metal_material(float r, float g, float b, float tint, float fuzz) {
	VoxMaterial m;
	m.type = 3;
	m.color = (cl_float3){r, g, b};
	m.v1 = tint;
	m.v2 = fuzz;
	m.v3 = 0;

	return m;
}

VoxMaterial create_dielectric_material(float r, float g, float b, float tint, float fuzz, float refIdx) {
	VoxMaterial m;
	m.type = 4;
	m.color = (cl_float3){r, g, b};
	m.v1 = tint;
	m.v2 = fuzz;
	m.v3 = refIdx;

	return m;
}