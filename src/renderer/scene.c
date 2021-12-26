#include "renderer.h"

static int _voxel_exists(int x, int y, int z) {
	for (int i = 0; i < ren.scene.voxelCount; i++) {
		Voxel voxel = ren.scene.voxels[i];
		if (voxel.pos.x == x && voxel.pos.y == y && voxel.pos.z == z) return 1;
	}

	return 0;
}

static int _get_voxel_idx(int x, int y, int z) {
	for (int i = 0; i < ren.scene.voxelCount; i++) {
		Voxel voxel = ren.scene.voxels[i];
		if (voxel.pos.x == x && voxel.pos.y == y && voxel.pos.z == z) return i;
	}

	return -1;
}

static void _remove_voxel(int x, int y, int z) {
	int idx = _get_voxel_idx(x, y, z);
	if (idx == -1) return;
	ren.scene.voxelCount--;
	ren.scene.allocationSize--;
	for (int i = idx; i < ren.scene.voxelCount; i++) ren.scene.voxels[i] = ren.scene.voxels[i + 1];
	ren.scene.voxels = realloc(ren.scene.voxels, sizeof(Voxel) * ren.scene.voxelCount);
}

static void _get_chunk_pos(int vx, int vy, int vz, int *cx, int *cy, int *cz) {
	*cx = vx / ren.scene.chunkSize;
	*cy = vy / ren.scene.chunkSize;
	*cz = vz / ren.scene.chunkSize;
}

static int _get_chunk_idx_from_vector(int vx, int vy, int vz) {
	int cx, cy, cz;
	_get_chunk_pos(vx, vy, vz, &cx, &cy, &cz);

	for (int i = 0; i < ren.scene.chunkCount; i++) {
		Chunk chunk = ren.scene.chunks[i];
		if (chunk.pos.x == cx && chunk.pos.y == cy && chunk.pos.z == cz) return i;
	}

	return -1;
}

static int _get_chunk_idx(int x, int y, int z) {
	for (int i = 0; i < ren.scene.chunkCount; i++) {
		Chunk chunk = ren.scene.chunks[i];
		if (chunk.pos.x == x && chunk.pos.y == y && chunk.pos.z == z) return i;
	}

	return -1;
}

static int _chunk_exists(int x, int y, int z) {
	for (int i = 0; i < ren.scene.chunkCount; i++) {
		Chunk chunk = ren.scene.chunks[i];
		if (chunk.pos.x == x && chunk.pos.y == y && chunk.pos.z == z) return 1;
	}

	return 0;
}

static void _create_chunk(int x, int y, int z) {
	Chunk chunk = (Chunk){(cl_int3){.x = x, .y = y, .z = z}, ren.scene.voxelCount, 0};

	ren.scene.chunkCount++;
	ren.scene.chunks = realloc(ren.scene.chunks, ren.scene.chunkCount * sizeof(Chunk));

	ren.scene.chunks[ren.scene.chunkCount - 1] = chunk;
}

static void _remove_chunk(int x, int y, int z) {
	int idx = _get_chunk_idx(x, y, z);
	if (idx == -1) return;
	ren.scene.chunkCount--;
	for (int i = idx; i < ren.scene.chunkCount; i++) ren.scene.chunks[i] = ren.scene.chunks[i + 1];
	ren.scene.chunks = realloc(ren.scene.chunks, sizeof(Chunk) * ren.scene.chunkCount);
}

static void _add_layers(int axis, int layers) {
	dbg("Adding layer");

	int voxCount = ren.scene.voxelCount;
	Voxel *oldVoxels = malloc(sizeof(Voxel) * voxCount);
	memcpy(oldVoxels, ren.scene.voxels, sizeof(Voxel) * voxCount);

	ren.scene.voxelCount = 0;
	ren.scene.allocationSize = 0;
	ren.scene.chunkCount = 0;

	pre_allocate_space_for_voxels(voxCount);

	for (int i = 0; i < voxCount; i++) {
		add_voxel(oldVoxels[i].pos.x + layers * (axis == 0),
				  oldVoxels[i].pos.y + layers * (axis == 1),
				  oldVoxels[i].pos.z + layers * (axis == 2),
				  oldVoxels[i].material);
	}

	ren.camera.pos.x += layers * (axis == 0);
	ren.camera.pos.y += layers * (axis == 1);
	ren.camera.pos.z += layers * (axis == 2);
}

Status set_output_properties(int width, int height) {
	safe_free(ren.image.data);
	ren.image.size = (cl_int2){.x = width, .y = height};
	ren.image.data = malloc(sizeof(cl_float3) * width * height);

	return SUCCESS;
}

Status set_background_properties(float red, float green, float blue, float brightness) {
	ren.scene.bgColor = (cl_float3){.x = red, .y = green, .z = blue};
	ren.scene.bgBrightness = brightness;

	return SUCCESS;
}

Status pre_allocate_space_for_voxels(int additionalSpaces) {
	ren.scene.allocationSize = ren.scene.voxelCount + additionalSpaces;
	dbg("Pre-allocating additional space for %d voxels (total: %d voxels)", additionalSpaces, ren.scene.allocationSize);
	ren.scene.voxels = realloc(ren.scene.voxels, ren.scene.allocationSize * sizeof(Voxel));

	return SUCCESS;
}

Status add_voxel(int x, int y, int z, VoxMaterial material) {
	if (x < 0) {
		_add_layers(0, -x);
		x = 0;
	}
	if (y < 0) {
		_add_layers(1, -y);
		y = 0;
	}
	if (z < 0) {
		_add_layers(2, -z);
		z = 0;
	}

	if (_voxel_exists(x, y, z)) remove_voxel(x, y, z);
	Voxel voxel = (Voxel){(cl_int3){x, y, z}, material};

	int cx, cy, cz;
	_get_chunk_pos(voxel.pos.x, voxel.pos.y, voxel.pos.z, &cx, &cy, &cz);

	int idx;
	if (!_chunk_exists(cx, cy, cz)) {
		_create_chunk(cx, cy, cz);
		idx = ren.scene.chunkCount - 1;
	} else {
		idx = _get_chunk_idx_from_vector(voxel.pos.x, voxel.pos.y, voxel.pos.z);
	}

	// resize voxel buffer
	ren.scene.voxelCount++;
	if (ren.scene.allocationSize < ren.scene.voxelCount) {
		ren.scene.voxels = realloc(ren.scene.voxels, ren.scene.voxelCount * sizeof(Voxel));
		ren.scene.allocationSize = ren.scene.voxelCount;
	}

	// make space for new voxel
	Chunk *chunk = &ren.scene.chunks[idx];
	int insertPos = chunk->firstVoxel + chunk->voxelCount;
	int voxelsToMove = ren.scene.voxelCount - insertPos - 1;
	if (insertPos != ren.scene.voxelCount - 1)
		memcpy(&ren.scene.voxels[insertPos + 1], &ren.scene.voxels[insertPos], sizeof(Voxel) * voxelsToMove);

	ren.scene.voxels[insertPos] = voxel;

	// adjust firstVoxel of affected chunks
	for (int i = idx + 1; i < ren.scene.chunkCount; i++) ren.scene.chunks[i].firstVoxel++;
	ren.scene.chunks[idx].voxelCount++;

	return SUCCESS;
}

Status remove_voxel(int x, int y, int z) {
	if (!_voxel_exists(x, y, z)) return FAILURE;
	_remove_voxel(x, y, z);
	int idx = _get_chunk_idx_from_vector(x, y, z);
	for (int i = idx + 1; i < ren.scene.chunkCount; i++) ren.scene.chunks[i].firstVoxel--;
	ren.scene.chunks[idx].voxelCount--;

	if (ren.scene.chunks[idx].voxelCount == 0) {
		int cx, cy, cz;
		_get_chunk_pos(x, y, z, &cx, &cy, &cz);
		_remove_chunk(cx, cy, cz);
	}

	return SUCCESS;
}

Status resize_chunks(int chunkSize) {
	if (chunkSize < 1) return FAILURE;

	ren.scene.chunkSize = chunkSize;

	int voxCount = ren.scene.voxelCount;
	Voxel *oldVoxels = malloc(sizeof(Voxel) * voxCount);
	memcpy(oldVoxels, ren.scene.voxels, sizeof(Voxel) * voxCount);

	ren.scene.voxelCount = 0;
	ren.scene.allocationSize = 0;
	ren.scene.chunkCount = 0;

	pre_allocate_space_for_voxels(voxCount);

	for (int i = 0; i < voxCount; i++)
		add_voxel(oldVoxels[i].pos.x, oldVoxels[i].pos.y, oldVoxels[i].pos.z, oldVoxels[i].material);

	safe_free(oldVoxels);
	ren.restartRender = 1;

	return SUCCESS;
}