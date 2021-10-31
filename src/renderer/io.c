#include "renderer.h"

void _create_new_scene() {
	msg("Creating new file %s", ren.fileName);

	ren.camera = (VoxCamera){(cl_float3){0, 0, -10}, (cl_float2){0, 0}, 1, 1, 0.001, 1000};
	ren.scene.bgColor = (cl_float3){0.5, 0.5, 1};
	ren.scene.bgBrightness = 0.5;

	ren.scene.voxelCount = 0;
	ren.scene.chunkCount = 0;
}

Status save_scene(char *fileName) {
	msg("Saving scene to %s", fileName);

	FILE *fp = fopen(fileName, "wb");

	// camera
	fwrite(&ren.camera.pos.x, sizeof(cl_float), 1, fp);
	fwrite(&ren.camera.pos.y, sizeof(cl_float), 1, fp);
	fwrite(&ren.camera.pos.z, sizeof(cl_float), 1, fp);

	fwrite(&ren.camera.rot.x, sizeof(cl_float), 1, fp);
	fwrite(&ren.camera.rot.y, sizeof(cl_float), 1, fp);

	fwrite(&ren.camera.sensorWidth, sizeof(cl_float), 1, fp);
	fwrite(&ren.camera.focalLength, sizeof(cl_float), 1, fp);
	fwrite(&ren.camera.aperture, sizeof(cl_float), 1, fp);
	fwrite(&ren.camera.exposure, sizeof(cl_float), 1, fp);

	// bg color
	fwrite(&ren.scene.bgColor.x, sizeof(cl_float), 1, fp);
	fwrite(&ren.scene.bgColor.y, sizeof(cl_float), 1, fp);
	fwrite(&ren.scene.bgColor.z, sizeof(cl_float), 1, fp);
	fwrite(&ren.scene.bgBrightness, sizeof(cl_float), 1, fp);

	// voxles
	fwrite(&ren.scene.voxelCount, sizeof(cl_int), 1, fp);
	fwrite(ren.scene.voxels, sizeof(Voxel), ren.scene.voxelCount, fp);

	fclose(fp);

	return SUCCESS;
}

Status load_scene(char *fileName) {
	if (fileName == NULL) {
		ren.fileName = malloc(1000);
		sprintf(ren.fileName, "%d", (int)time(NULL));

		_create_new_scene();
		return FAILURE;
	}

	ren.fileName = fileName;
	FILE *fp = fopen(fileName, "rb");

	if (fp == NULL) {
		_create_new_scene();
		return FAILURE;
	}

	msg("Reading scene from %s", ren.fileName);

	// camera
	fread(&ren.camera.pos.x, sizeof(cl_float), 1, fp);
	fread(&ren.camera.pos.y, sizeof(cl_float), 1, fp);
	fread(&ren.camera.pos.z, sizeof(cl_float), 1, fp);

	fread(&ren.camera.rot.x, sizeof(cl_float), 1, fp);
	fread(&ren.camera.rot.y, sizeof(cl_float), 1, fp);

	fread(&ren.camera.sensorWidth, sizeof(cl_float), 1, fp);
	fread(&ren.camera.focalLength, sizeof(cl_float), 1, fp);
	fread(&ren.camera.aperture, sizeof(cl_float), 1, fp);
	fread(&ren.camera.exposure, sizeof(cl_float), 1, fp);

	// bg color
	fread(&ren.scene.bgColor.x, sizeof(cl_float), 1, fp);
	fread(&ren.scene.bgColor.y, sizeof(cl_float), 1, fp);
	fread(&ren.scene.bgColor.z, sizeof(cl_float), 1, fp);
	fread(&ren.scene.bgBrightness, sizeof(cl_float), 1, fp);

	// voxles
	cl_int voxCount;
	fread(&voxCount, sizeof(cl_int), 1, fp);
	dbg("Voxel count: %d", voxCount);

	Voxel *tmp = malloc(sizeof(Voxel) * voxCount);
	fread(tmp, sizeof(Voxel), voxCount, fp);

	ren.scene.voxelCount = 0;
	ren.scene.chunkCount = 0;

	for (int i = 0; i < voxCount; i++) {
		add_voxel(tmp[i].pos.x, tmp[i].pos.y, tmp[i].pos.z, tmp[i].material);
	}

	free(tmp);
	fclose(fp);

	return SUCCESS;
}