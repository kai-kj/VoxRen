#include "renderer.h"

VoxMaterial create_light_source_material(float r, float g, float b,
										 float brightness) {
	VoxMaterial m;

	m.type = 1;
	m.color = (cl_float3){r, g, b};
	m.details.lightSource.brightness = brightness;

	return m;
}

VoxMaterial create_lambertian_material(float r, float g, float b) {
	VoxMaterial m;

	m.type = 2;
	m.color = (cl_float3){r, g, b};

	return m;
}

VoxMaterial create_metal_material(float r, float g, float b, float tint,
								  float fuzz) {
	VoxMaterial m;

	m.type = 3;
	m.color = (cl_float3){r, g, b};
	m.details.metal.tint = tint;
	m.details.metal.fuzz = fuzz;

	return m;
}

VoxMaterial create_dielectric_material(float r, float g, float b, float tint,
									   float fuzz, float refIdx) {
	VoxMaterial m;

	m.type = 3;
	m.color = (cl_float3){r, g, b};
	m.details.dielectric.tint = tint;
	m.details.dielectric.fuzz = fuzz;
	m.details.dielectric.refIdx = refIdx;

	return m;
}