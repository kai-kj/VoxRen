#ifndef RENDERER_STRUCTS_H
#define RENDERER_STRUCTS_H

#include <CL/cl.h>

typedef struct CLProgram {
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem imageBuff;
	cl_mem voxelBuff;
	cl_mem chunkBuff;
	cl_mem lookingAtBuff;
} CLProgram;

typedef struct CLImage {
	cl_int2 size;
	cl_float3 *data;
} CLImage;

typedef struct VoxMaterial {
	cl_int type;
	cl_float3 color;

	cl_float v1;
	cl_float v2;
	cl_float v3;

} VoxMaterial;

typedef struct Voxel {
	cl_int3 pos;
	VoxMaterial material;
} Voxel;

typedef struct Chunk {
	cl_int3 pos;
	cl_int firstVoxel;
	cl_int voxelCount;
} Chunk;

typedef struct Scene {
	cl_int voxelCount;
	Voxel *voxels;

	cl_int chunkSize;
	cl_int chunkCount;
	Chunk *chunks;

	cl_float3 bgColor;
	cl_float bgBrightness;
} Scene;

typedef struct VoxCamera {
	cl_float3 pos;
	cl_float2 rot;
	cl_float sensorWidth;
	cl_float focalLength;
	cl_float aperture;
	cl_float exposure;
} VoxCamera;

typedef struct Renderer {
	int stopRender;
	int restartRender;
	int readFirstFrame;

	double dt;
	double prevTime;

	int lookingAt;
	cl_int2 mousePos;
	cl_int3 lookingAtPos;
	cl_int3 lookingAtNormal;
	cl_float lookingAtDistance;

	CLProgram program;
	CLImage image;
	Scene scene;
	VoxCamera camera;

	char *fileName;
} Renderer;

#endif