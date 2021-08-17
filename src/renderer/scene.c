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

static int _get_voxel_index(int x, int y, int z) {
	for (int i = 0; i < r.scene.voxelCount; i++) {
		Voxel voxel = r.scene.voxels[i];
		if (voxel.pos.x == x && voxel.pos.y == y && voxel.pos.z == z)
			return i;
	}

	return -1;
}

static void get_chunk_pos(Voxel voxel, int *x, int *y, int *z) {
	*x = voxel.pos.x / r.scene.chunkSize;
	*y = voxel.pos.y / r.scene.chunkSize;
	*z = voxel.pos.z / r.scene.chunkSize;
}

static int _get_chunk_idx(Voxel voxel) {
	int cx, cy, cz;
	get_chunk_pos(voxel, &cx, &cy, &cz);

	for (int i = 0; i < r.scene.chunkCount; i++) {
		Chunk chunk = r.scene.chunks[i];
		if (chunk.pos.x == cx && chunk.pos.y == cy && chunk.pos.z == cz)
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
	msg("creating new chunk %d (%d, %d, %d)\n", r.scene.chunkCount, x, y, z);
	Chunk chunk =
		(Chunk){(cl_int3){.x = x, .y = y, .z = z}, r.scene.voxelCount, 0};

	r.scene.chunkCount++;
	r.scene.chunks =
		realloc(r.scene.chunks, r.scene.chunkCount * sizeof(Chunk));

	r.scene.chunks[r.scene.chunkCount - 1] = chunk;
}

//---- public ----------------------------------------------------------------//

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
	get_chunk_pos(voxel, &cx, &cy, &cz);

	if (!_chunk_exists(cx, cy, cz)) {
		_create_chunk(cx, cy, cz);
	}

	int idx = _get_chunk_idx(voxel);

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

	// msg("inserting voxel at %d/%d\n", insertPos, r.scene.voxelCount - 1);

	r.scene.voxels[insertPos] = voxel;

	// adjust firstVoxel of affected chunks
	for (int i = idx + 1; i < r.scene.chunkCount; i++) {
		r.scene.chunks[i].firstVoxel++;
	}

	r.scene.chunks[idx].voxelCount++;

	return RENDERER_SUCCESS;
}

RendererStatus remove_voxel(int x, int y, int z) {
	// int idx = _get_voxel_index(x, y, z);

	// if (idx == -1)
	// 	return RENDERER_FAILURE;

	// r.scene.voxelCount--;

	// for (int i = idx; i < r.scene.voxelCount; i++) {
	// 	r.scene.voxels[i] = r.scene.voxels[i + 1];
	// }

	// r.scene.voxels =
	// 	realloc(r.scene.voxels, sizeof(Voxel) * r.scene.voxelCount);

	// return RENDERER_SUCCESS;
}

RendererStatus print_scene_state() {
	msg("Scene:\n");

	msg("  Voxels (chunk size: %d, chunk count: %d, voxel count: %d):\n",
		r.scene.chunkSize, r.scene.chunkCount, r.scene.voxelCount);

	for (int i = 0; i < r.scene.chunkCount; i++) {
		Chunk chunk = r.scene.chunks[i];

		msg("    C(%d, %d, %d):\n", chunk.pos.x, chunk.pos.y, chunk.pos.z);

		int lastVoxel = chunk.firstVoxel + chunk.voxelCount;
		for (int j = chunk.firstVoxel; j < lastVoxel; j++) {
			Voxel voxel = r.scene.voxels[j];
			msg("      V(%d, %d, %d)\n", voxel.pos.x, voxel.pos.y, voxel.pos.z);
		}
	}

	return RENDERER_SUCCESS;
}