#include "renderer.h"

Status save_scene_to_file(char *fileName) {
	msg("Saving scene to %s", fileName);

	FILE *fp = fopen(fileName, "wb");

	// camera
	fwrite(&ren.camera.pos.x, sizeof(float), 1, fp);
	fwrite(&ren.camera.pos.y, sizeof(float), 1, fp);
	fwrite(&ren.camera.pos.z, sizeof(float), 1, fp);

	fwrite(&ren.camera.rot.x, sizeof(float), 1, fp);
	fwrite(&ren.camera.rot.y, sizeof(float), 1, fp);

	fwrite(&ren.camera.sensorWidth, sizeof(float), 1, fp);
	fwrite(&ren.camera.focalLength, sizeof(float), 1, fp);
	fwrite(&ren.camera.aperture, sizeof(float), 1, fp);
	fwrite(&ren.camera.exposure, sizeof(float), 1, fp);

	// bg color
	fwrite(&ren.scene.bgColor.x, sizeof(float), 1, fp);
	fwrite(&ren.scene.bgColor.y, sizeof(float), 1, fp);
	fwrite(&ren.scene.bgColor.z, sizeof(float), 1, fp);
	fwrite(&ren.scene.bgBrightness, sizeof(float), 1, fp);

	// voxels
	fwrite(&ren.scene.voxelCount, sizeof(int), 1, fp);

	for (int i = 0; i < ren.scene.voxelCount; i++) {
		Voxel *v = &ren.scene.voxels[i];
		fwrite(&v->pos.x, sizeof(int), 1, fp);
		fwrite(&v->pos.y, sizeof(int), 1, fp);
		fwrite(&v->pos.z, sizeof(int), 1, fp);

		fwrite(&v->material.type, sizeof(int), 1, fp);

		fwrite(&v->material.color.x, sizeof(float), 1, fp);
		fwrite(&v->material.color.y, sizeof(float), 1, fp);
		fwrite(&v->material.color.z, sizeof(float), 1, fp);

		fwrite(&v->material.v1, sizeof(float), 1, fp);
		fwrite(&v->material.v2, sizeof(float), 1, fp);
		fwrite(&v->material.v3, sizeof(float), 1, fp);
	}

	fclose(fp);

	return SUCCESS;
}

Status load_scene_from_file(char *fileName) {
	msg("Reading scene from %s", fileName);

	ren.fileName = fileName;
	FILE *fp = fopen(fileName, "rb");

	if (fp != NULL) {

		// camera
		fread(&ren.camera.pos.x, sizeof(float), 1, fp);
		fread(&ren.camera.pos.y, sizeof(float), 1, fp);
		fread(&ren.camera.pos.z, sizeof(float), 1, fp);

		float rx, ry;
		fread(&rx, sizeof(float), 1, fp);
		fread(&ry, sizeof(float), 1, fp);

		set_camera_pos(ren.camera.pos.x, ren.camera.pos.y, ren.camera.pos.z, rx, ry);

		fread(&ren.camera.sensorWidth, sizeof(float), 1, fp);
		fread(&ren.camera.focalLength, sizeof(float), 1, fp);
		fread(&ren.camera.aperture, sizeof(float), 1, fp);
		fread(&ren.camera.exposure, sizeof(float), 1, fp);

		// bg color
		fread(&ren.scene.bgColor.x, sizeof(float), 1, fp);
		fread(&ren.scene.bgColor.y, sizeof(float), 1, fp);
		fread(&ren.scene.bgColor.z, sizeof(float), 1, fp);
		fread(&ren.scene.bgBrightness, sizeof(float), 1, fp);

		// voxles
		int voxCount;
		fread(&voxCount, sizeof(int), 1, fp);
		dbg("Voxel count: %d", voxCount);

		Voxel *tmp = malloc(sizeof(Voxel) * voxCount);

		printf("\e[?25l"); // hide cursor

		for (int i = 0; i < voxCount; i++) {
			printf("\r      > Reading voxel [%d/%d] (%02.2f%%)", i, voxCount, (float)i / (float)voxCount * 100.0);

			Voxel *v = &tmp[i];
			fread(&v->pos.x, sizeof(int), 1, fp);
			fread(&v->pos.y, sizeof(int), 1, fp);
			fread(&v->pos.z, sizeof(int), 1, fp);

			fread(&v->material.type, sizeof(int), 1, fp);

			fread(&v->material.color.x, sizeof(float), 1, fp);
			fread(&v->material.color.y, sizeof(float), 1, fp);
			fread(&v->material.color.z, sizeof(float), 1, fp);

			fread(&v->material.v1, sizeof(float), 1, fp);
			fread(&v->material.v2, sizeof(float), 1, fp);
			fread(&v->material.v3, sizeof(float), 1, fp);
		}

		printf("\n");

		ren.scene.voxelCount = 0;
		ren.scene.chunkCount = 0;

		for (int i = 0; i < voxCount; i++) {
			printf("\r      > Adding voxel [%d/%d] (%02.2f%%)", i, voxCount, (float)i / (float)voxCount * 100.0);

			add_voxel(tmp[i].pos.x, tmp[i].pos.y, tmp[i].pos.z, tmp[i].material);
		}

		printf("\n");

		printf("\e[?25h"); // show cursor

		free(tmp);
		fclose(fp);

	} else {
		ren.camera = (VoxCamera){(cl_float3){0, 0, -10}, (cl_float2){0, 0}, 1, 1, 0.001, 1000};
		ren.scene.bgColor = (cl_float3){0.5, 0.5, 1};
		ren.scene.bgBrightness = 0.5;

		ren.scene.voxelCount = 0;
		ren.scene.chunkCount = 0;
	}

	return SUCCESS;
}