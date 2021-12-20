#include "renderer.h"

#define FILE_NAME "data/kernel/pathtracer.cl"
#define KERNEL_NAME "pathtracer"
#define ARGS "-Werror -cl-mad-enable -cl-no-signed-zeros -cl-fast-relaxed-math"
#define CHUNK_SIZE 3

Status setup_renderer_args() {
	int imageSize = ren.image.size.x * ren.image.size.y;

	cl_destroy_buffer(ren.program.imageBuff);
	ren.program.imageBuff = cl_create_buffer(ren.program.context, sizeof(cl_float3) * imageSize, CL_MEM_READ_WRITE);

	cl_destroy_buffer(ren.program.voxelBuff);
	ren.program.voxelBuff =
		cl_create_buffer(ren.program.context, sizeof(Voxel) * ren.scene.voxelCount, CL_MEM_READ_ONLY);
	clEnqueueWriteBuffer(ren.program.queue, ren.program.voxelBuff, CL_TRUE, 0, sizeof(Voxel) * ren.scene.voxelCount,
						 ren.scene.voxels, 0, NULL, NULL);

	cl_destroy_buffer(ren.program.chunkBuff);
	ren.program.chunkBuff =
		cl_create_buffer(ren.program.context, sizeof(Chunk) * ren.scene.chunkCount, CL_MEM_READ_ONLY);
	clEnqueueWriteBuffer(ren.program.queue, ren.program.chunkBuff, CL_TRUE, 0, sizeof(Chunk) * ren.scene.chunkCount,
						 ren.scene.chunks, 0, NULL, NULL);

	cl_set_kernel_arg(ren.program.kernel, 0, sizeof(cl_int2), &ren.image.size);
	cl_set_kernel_arg(ren.program.kernel, 1, sizeof(cl_mem), &ren.program.imageBuff);
	cl_set_kernel_arg(ren.program.kernel, 2, sizeof(cl_int), &ren.scene.voxelCount);
	cl_set_kernel_arg(ren.program.kernel, 3, sizeof(cl_mem), &ren.program.voxelBuff);
	cl_set_kernel_arg(ren.program.kernel, 4, sizeof(cl_int), &ren.scene.chunkSize);
	cl_set_kernel_arg(ren.program.kernel, 5, sizeof(cl_int), &ren.scene.chunkCount);
	cl_set_kernel_arg(ren.program.kernel, 6, sizeof(cl_mem), &ren.program.chunkBuff);
	cl_set_kernel_arg(ren.program.kernel, 7, sizeof(cl_float3), &ren.scene.bgColor);
	cl_set_kernel_arg(ren.program.kernel, 8, sizeof(cl_float), &ren.scene.bgBrightness);
	cl_set_kernel_arg(ren.program.kernel, 9, sizeof(VoxCamera), &ren.camera);

	return SUCCESS;
}

Status render_frame(int sampleNumber) {
	cl_ulong seed = rand();

	// non-constant arguments
	cl_int3 lookingAt[3] = {ren.lookingAtPos, ren.lookingAtNormal, (cl_int3){ren.mousePos.x, ren.mousePos.y, 0}};

	cl_destroy_buffer(ren.program.lookingAtBuff);
	ren.program.lookingAtBuff = cl_create_buffer(ren.program.context, sizeof(cl_int3) * 3, CL_MEM_READ_WRITE);
	clEnqueueWriteBuffer(ren.program.queue, ren.program.lookingAtBuff, CL_TRUE, 0, sizeof(cl_int3) * 3, lookingAt, 0,
						 NULL, NULL);

	cl_set_kernel_arg(ren.program.kernel, 10, sizeof(cl_mem), &ren.program.lookingAtBuff);
	cl_set_kernel_arg(ren.program.kernel, 11, sizeof(cl_int), &sampleNumber);
	cl_set_kernel_arg(ren.program.kernel, 12, sizeof(cl_ulong), &seed);

	cl_int ret = cl_run_kernel(ren.program.queue, ren.program.kernel, ren.image.size.x * ren.image.size.y);

	if (ret != CL_SUCCESS) {
		cl_print_kernel_run_error(ret);
		return FAILURE;
	}

	cl_read_buffer(ren.program.queue, ren.program.imageBuff, 0, sizeof(cl_float3) * ren.image.size.x * ren.image.size.y,
				   ren.image.data);

	cl_int3 data[3];
	cl_read_buffer(ren.program.queue, ren.program.lookingAtBuff, 0, sizeof(cl_int3) * 3, data);

	if (data[2].z == -1) {
		ren.lookingAt = 0;
	} else {
		ren.lookingAt = 1;
		ren.lookingAtPos = data[0];
		ren.lookingAtNormal = data[1];
		ren.lookingAtDistance = (float)data[2].z / 100.0;
	}

	double currentTime = get_time();
	ren.dt = currentTime - ren.prevTime;
	ren.prevTime = currentTime;

	clFinish(ren.program.queue);

	return SUCCESS;
}

static void *_start_renderer_loop() {
	ren.restartRender = 1;

	int i;
	while (!ren.stopRender) {
		if (ren.restartRender) {
			ren.restartRender = 0;
			ren.readFirstFrame = 0;
			i = 0;
			setup_renderer_args();
		}

		if (render_frame(i) == FAILURE) panic("Failed to run kernel");

		i++;
	}

	ren.stopRender = 0;
	pthread_exit(NULL);
}

Status create_renderer() {
	msg("creating renderer");

	srand(time(NULL));

	ren.scene.voxelCount = 0;
	ren.scene.chunkCount = 0;
	ren.program.voxelBuff = NULL;
	ren.program.imageBuff = NULL;
	ren.scene.voxels = NULL;
	ren.scene.chunks = NULL;

	ren.scene.chunkSize = CHUNK_SIZE;

	ren.camera = (VoxCamera){.sensorWidth = 1, .focalLength = 1, .aperture = 1, .exposure = 1};

	// TODO: check platform count
	cl_uint platformNum;
	clGetPlatformIDs(1, &ren.program.platform, &platformNum);
	cl_uint deviceNum;
	clGetDeviceIDs(ren.program.platform, CL_DEVICE_TYPE_GPU, 2, &ren.program.device, &deviceNum);

	ren.program.context = clCreateContext(0, 1, &ren.program.device, NULL, NULL, NULL);
	ren.program.queue = clCreateCommandQueueWithProperties(ren.program.context, ren.program.device, 0, NULL);

	char *source = read_file(FILE_NAME);

	if (source == NULL) {
		cl_print_source_read_error();
		destroy_renderer();
		return FAILURE;
	}

	ren.program.program = clCreateProgramWithSource(ren.program.context, 1, (const char **)&source, NULL, NULL);

	free(source);

	if (clBuildProgram(ren.program.program, 0, NULL, ARGS, NULL, NULL) != CL_SUCCESS) {
		cl_print_program_build_error(ren.program.device, ren.program.program);
		destroy_renderer();
		return FAILURE;
	}

	ren.program.kernel = clCreateKernel(ren.program.program, KERNEL_NAME, NULL);

	return SUCCESS;
}

Status destroy_renderer() {
	msg("Destroying renderer");

	safe_free(ren.scene.voxels);
	safe_free(ren.image.data);
	cl_destroy_buffer(ren.program.voxelBuff);
	cl_destroy_buffer(ren.program.imageBuff);
	cl_destroy_buffer(ren.program.chunkBuff);
	cl_destroy_buffer(ren.program.lookingAtBuff);
	clReleaseProgram(ren.program.program);
	clReleaseKernel(ren.program.kernel);
	clReleaseContext(ren.program.context);

	return SUCCESS;
}

Status begin_rendering() {
	msg("Starting render");

	ren.restartRender = 1;
	ren.stopRender = 0;

	pthread_t thread;
	pthread_create(&thread, NULL, _start_renderer_loop, NULL);

	return SUCCESS;
}

Status end_rendering() {
	msg("Stopping render");

	ren.stopRender = 1;
	return SUCCESS;
}