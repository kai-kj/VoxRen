#include "renderer.h"

//---- private ---------------------------------------------------------------//

static int _voxel_exists(int x, int y, int z) {
	for (int i = 0; i < r.scene.voxelCount; i++) {
		Voxel voxel = r.scene.voxels[i];
		if (voxel.pos.x == x && voxel.pos.y == y && voxel.pos.z == z)
			return 1;
	}

	return 0;
}

static int _get_voxel_idx(int x, int y, int z) {
	for (int i = 0; i < r.scene.voxelCount; i++) {
		Voxel voxel = r.scene.voxels[i];
		if (voxel.pos.x == x && voxel.pos.y == y && voxel.pos.z == z)
			return i;
	}

	return -1;
}

static void _remove_voxel(int x, int y, int z) {
	int idx = _get_voxel_idx(x, y, z);

	if (idx == -1)
		return;

	r.scene.voxelCount--;

	for (int i = idx; i < r.scene.voxelCount; i++) {
		r.scene.voxels[i] = r.scene.voxels[i + 1];
	}

	r.scene.voxels =
		realloc(r.scene.voxels, sizeof(Voxel) * r.scene.voxelCount);
}

static void _get_chunk_pos(int vx, int vy, int vz, int *cx, int *cy, int *cz) {
	*cx = vx / r.scene.chunkSize;
	*cy = vy / r.scene.chunkSize;
	*cz = vz / r.scene.chunkSize;
}

static int _get_chunk_idx_from_vector(int vx, int vy, int vz) {
	int cx, cy, cz;
	_get_chunk_pos(vx, vy, vz, &cx, &cy, &cz);

	for (int i = 0; i < r.scene.chunkCount; i++) {
		Chunk chunk = r.scene.chunks[i];
		if (chunk.pos.x == cx && chunk.pos.y == cy && chunk.pos.z == cz)
			return i;
	}

	return -1;
}

static int _get_chunk_idx(int x, int y, int z) {
	for (int i = 0; i < r.scene.chunkCount; i++) {
		Chunk chunk = r.scene.chunks[i];
		if (chunk.pos.x == x && chunk.pos.y == y && chunk.pos.z == z)
			return i;
	}

	return -1;
}

static int _chunk_exists(int x, int y, int z) {
	for (int i = 0; i < r.scene.chunkCount; i++) {
		Chunk chunk = r.scene.chunks[i];
		if (chunk.pos.x == x && chunk.pos.y == y && chunk.pos.z == z)
			return 1;
	}

	return 0;
}

static void _create_chunk(int x, int y, int z) {
	dbg("new chunk (%d, %d, %d), total: %d", x, y, z, r.scene.chunkCount + 1);
	Chunk chunk =
		(Chunk){(cl_int3){.x = x, .y = y, .z = z}, r.scene.voxelCount, 0};

	r.scene.chunkCount++;
	r.scene.chunks =
		realloc(r.scene.chunks, r.scene.chunkCount * sizeof(Chunk));

	r.scene.chunks[r.scene.chunkCount - 1] = chunk;
}

static void _remove_chunk(int x, int y, int z) {
	dbg("removing chunk (%d, %d, %d), total: %d", x, y, z,
		r.scene.chunkCount - 1);

	int idx = _get_chunk_idx(x, y, z);

	if (idx == -1)
		return;

	r.scene.chunkCount--;

	for (int i = idx; i < r.scene.chunkCount; i++) {
		r.scene.chunks[i] = r.scene.chunks[i + 1];
	}

	r.scene.chunks =
		realloc(r.scene.chunks, sizeof(Chunk) * r.scene.chunkCount);
}

//---- public ----------------------------------------------------------------//

RendererStatus set_output_properties(int width, int height) {
	safe_free(r.image.data);
	r.image.size = (cl_int2){.x = width, .y = height};
	r.image.data = malloc(sizeof(cl_float3) * width * height);

	return RENDERER_SUCCESS;
}

RendererStatus set_background_properties(float red, float green, float blue,
										 float brightness) {
	r.scene.bgColor = (cl_float3){.x = red, .y = green, .z = blue};
	r.scene.bgBrightness = brightness;

	return RENDERER_SUCCESS;
}

RendererStatus add_voxel(int x, int y, int z, Material material) {
	if (x < 0 || y < 0 || z < 0)
		return RENDERER_FAILURE;

	if (_voxel_exists(x, y, z))
		remove_voxel(x, y, z);

	Voxel voxel = (Voxel){(cl_int3){x, y, z}, material};

	int cx, cy, cz;
	_get_chunk_pos(voxel.pos.x, voxel.pos.y, voxel.pos.z, &cx, &cy, &cz);

	if (!_chunk_exists(cx, cy, cz)) {
		_create_chunk(cx, cy, cz);
	}

	int idx = _get_chunk_idx_from_vector(voxel.pos.x, voxel.pos.y, voxel.pos.z);

	// resize voxel buffer
	r.scene.voxelCount++;
	r.scene.voxels =
		realloc(r.scene.voxels, r.scene.voxelCount * sizeof(Voxel));

	// make space for new voxel
	Chunk *chunk = &r.scene.chunks[idx];
	int insertPos = chunk->firstVoxel + chunk->voxelCount;
	int voxelsToMove = r.scene.voxelCount - insertPos - 1;
	memcpy(&r.scene.voxels[insertPos + 1], &r.scene.voxels[insertPos],
		   sizeof(Voxel) * voxelsToMove);

	r.scene.voxels[insertPos] = voxel;

	// adjust firstVoxel of affected chunks
	for (int i = idx + 1; i < r.scene.chunkCount; i++) {
		r.scene.chunks[i].firstVoxel++;
	}

	r.scene.chunks[idx].voxelCount++;

	return RENDERER_SUCCESS;
}

RendererStatus remove_voxel(int x, int y, int z) {
	// remove voxel from voxel array
	_remove_voxel(x, y, z);

	// adjust chunks
	int idx = _get_chunk_idx_from_vector(x, y, z);

	for (int i = idx + 1; i < r.scene.chunkCount; i++) {
		r.scene.chunks[i].firstVoxel--;
	}

	r.scene.chunks[idx].voxelCount--;

	if (r.scene.chunks[idx].voxelCount == 0) {
		int cx, cy, cz;
		_get_chunk_pos(x, y, z, &cx, &cy, &cz);
		_remove_chunk(cx, cy, cz);
	}

	return RENDERER_SUCCESS;
}