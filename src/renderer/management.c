#include "renderer.h"

#define FILE_NAME "data/kernel/pathtracer.cl"
#define KERNEL_NAME "pathtracer"
#define ARGS "-Werror -cl-mad-enable -cl-no-signed-zeros -cl-fast-relaxed-math"
#define CHUNK_SIZE 3
#define QUICK_PREVIEW 0

static k_Image *_CLImage_to_k_Image(CLImage clImage) {
	k_Image *image = k_create_image(clImage.size.x, clImage.size.y);

	for (int i = 0; i < image->width * image->height; i++) {
		if (clImage.data[i].x < 0) clImage.data[i].x = 0;
		if (clImage.data[i].y < 0) clImage.data[i].y = 0;
		if (clImage.data[i].z < 0) clImage.data[i].z = 0;

		if (clImage.data[i].x > 1) clImage.data[i].x = 1;
		if (clImage.data[i].y > 1) clImage.data[i].y = 1;
		if (clImage.data[i].z > 1) clImage.data[i].z = 1;

		image->data[i * 3 + 0] = (int)(clImage.data[i].x * 255);
		image->data[i * 3 + 1] = (int)(clImage.data[i].y * 255);
		image->data[i * 3 + 2] = (int)(clImage.data[i].z * 255);
	}

	return image;
}

static void _setup_renderer_args() {
	int imageSize = r.image.size.x * r.image.size.y;

	cl_destroy_buffer(r.program.imageBuff);
	r.program.imageBuff = cl_create_buffer(r.program.context, sizeof(cl_float3) * imageSize, CL_MEM_READ_WRITE);

	cl_destroy_buffer(r.program.voxelBuff);
	r.program.voxelBuff = cl_create_buffer(r.program.context, sizeof(Voxel) * r.scene.voxelCount, CL_MEM_READ_ONLY);

	clEnqueueWriteBuffer(r.program.queue, r.program.voxelBuff, CL_TRUE, 0, sizeof(Voxel) * r.scene.voxelCount,
						 r.scene.voxels, 0, NULL, NULL);

	cl_destroy_buffer(r.program.chunkBuff);
	r.program.chunkBuff = cl_create_buffer(r.program.context, sizeof(Chunk) * r.scene.chunkCount, CL_MEM_READ_ONLY);

	clEnqueueWriteBuffer(r.program.queue, r.program.chunkBuff, CL_TRUE, 0, sizeof(Chunk) * r.scene.chunkCount,
						 r.scene.chunks, 0, NULL, NULL);

	cl_set_kernel_arg(r.program.kernel, 0, sizeof(cl_int2), &r.image.size);
	cl_set_kernel_arg(r.program.kernel, 1, sizeof(cl_mem), &r.program.imageBuff);
	cl_set_kernel_arg(r.program.kernel, 2, sizeof(cl_int), &r.scene.voxelCount);
	cl_set_kernel_arg(r.program.kernel, 3, sizeof(cl_mem), &r.program.voxelBuff);
	cl_set_kernel_arg(r.program.kernel, 4, sizeof(cl_int), &r.scene.chunkSize);
	cl_set_kernel_arg(r.program.kernel, 5, sizeof(cl_int), &r.scene.chunkCount);
	cl_set_kernel_arg(r.program.kernel, 6, sizeof(cl_mem), &r.program.chunkBuff);
	cl_set_kernel_arg(r.program.kernel, 7, sizeof(cl_float3), &r.scene.bgColor);
	cl_set_kernel_arg(r.program.kernel, 8, sizeof(cl_float3), &r.scene.bgBrightness);
	cl_set_kernel_arg(r.program.kernel, 9, sizeof(VoxCamera), &r.camera);
}

static RendererStatus _render_frame(int sampleNumber) {
	cl_ulong seed = rand();
	cl_int preview = !sampleNumber;

	// non-constant arguments
	cl_set_kernel_arg(r.program.kernel, 10, sizeof(cl_int), &sampleNumber);
	cl_set_kernel_arg(r.program.kernel, 11, sizeof(cl_ulong), &seed);
	cl_set_kernel_arg(r.program.kernel, 12, sizeof(cl_int), &preview);

	cl_int ret = cl_run_kernel(r.program.queue, r.program.kernel, r.image.size.x * r.image.size.y);

	if (ret != CL_SUCCESS) {
		cl_print_kernel_run_error(ret);
		return RENDERER_FAILURE;
	}

	cl_read_buffer(r.program.queue, r.program.imageBuff, 0, sizeof(cl_float3) * r.image.size.x * r.image.size.y,
				   r.image.data);

	double currentTime = get_time();
	r.dt = currentTime - r.prevTime;
	r.prevTime = currentTime;

	clFinish(r.program.queue);

	return RENDERER_SUCCESS;
}

static void *_start_renderer_loop() {
	r.restartRender = 1;

	int i;
	while (!r.stopRender) {
		if (r.restartRender) {
			r.restartRender = 0;
			r.readFirstFrame = 0;
			i = 0;
			_setup_renderer_args();
		}

		if (QUICK_PREVIEW && !r.readFirstFrame) i = 0;
		if (!QUICK_PREVIEW && i == 0) i = 1;
		if (_render_frame(i) == RENDERER_FAILURE) panic("Failed to run kernel");

		i++;
	}

	r.stopRender = 0;
	pthread_exit(NULL);
}

RendererStatus create_renderer() {
	msg("creating renderer");

	srand(time(NULL));

	r.scene.voxelCount = 0;
	r.scene.chunkCount = 0;
	r.program.voxelBuff = NULL;
	r.program.imageBuff = NULL;
	r.scene.voxels = NULL;
	r.scene.chunks = NULL;

	r.scene.chunkSize = CHUNK_SIZE;

	r.camera = (VoxCamera){.sensorWidth = 1, .focalLength = 1, .aperture = 1, .exposure = 1};

	// TODO: check platform count
	cl_uint platformNum;
	clGetPlatformIDs(1, &r.program.platform, &platformNum);
	cl_uint deviceNum;
	clGetDeviceIDs(r.program.platform, CL_DEVICE_TYPE_GPU, 2, &r.program.device, &deviceNum);

	r.program.context = clCreateContext(0, 1, &r.program.device, NULL, NULL, NULL);
	r.program.queue = clCreateCommandQueueWithProperties(r.program.context, r.program.device, 0, NULL);

	char *source = read_file(FILE_NAME);

	if (source == NULL) {
		cl_print_source_read_error();
		destroy_renderer();
		return RENDERER_FAILURE;
	}

	r.program.program = clCreateProgramWithSource(r.program.context, 1, (const char **)&source, NULL, NULL);

	free(source);

	if (clBuildProgram(r.program.program, 0, NULL, ARGS, NULL, NULL) != CL_SUCCESS) {
		cl_print_program_build_error(r.program.device, r.program.program);
		destroy_renderer();
		return RENDERER_FAILURE;
	}

	r.program.kernel = clCreateKernel(r.program.program, KERNEL_NAME, NULL);

	return RENDERER_SUCCESS;
}

RendererStatus destroy_renderer() {
	msg("Destroying renderer");

	safe_free(r.scene.voxels);
	safe_free(r.image.data);
	cl_destroy_buffer(r.program.voxelBuff);
	cl_destroy_buffer(r.program.imageBuff);
	clReleaseProgram(r.program.program);
	clReleaseKernel(r.program.kernel);
	clReleaseContext(r.program.context);

	return RENDERER_SUCCESS;
}

RendererStatus begin_rendering() {
	msg("Starting render");

	r.restartRender = 1;
	r.stopRender = 0;

	pthread_t thread;
	pthread_create(&thread, NULL, _start_renderer_loop, NULL);

	return RENDERER_SUCCESS;
}

RendererStatus end_rendering() {
	msg("Stopping render");

	r.stopRender = 1;
	return RENDERER_SUCCESS;
}

k_Image *get_image() {
	k_Image *image = _CLImage_to_k_Image(r.image);
	k_gamma_correct_image(image);
	return image;
}