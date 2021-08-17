#include "renderer.h"

#define FILE_NAME "data/kernel/pathtracer.cl"
#define KERNEL_NAME "pathtracer"
#define ARGS "-Werror -cl-mad-enable -cl-no-signed-zeros -cl-fast-relaxed-math"
#define CHUNK_SIZE 3

//---- private ---------------------------------------------------------------//

static void _print_source_read_error() { err("Failed to read source file"); }

static void _print_program_build_error(cl_device_id device,
									   cl_program program) {
	char buff[0x100000];
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buff),
						  buff, NULL);
	err("Failed to build program: %s", buff);
}

static void _print_kernel_run_error(cl_int ret) {
	char buff[0x100000];

	switch (ret) {
	case CL_INVALID_PROGRAM_EXECUTABLE:
		sprintf(buff, "INVALID_PROGRAM_EXECUTABLE");
		break;
	case CL_INVALID_COMMAND_QUEUE:
		sprintf(buff, "INVALID_COMMAND_QUEUE");
		break;
	case CL_INVALID_KERNEL:
		sprintf(buff, "INVALID_KERNEL");
		break;
	case CL_INVALID_CONTEXT:
		sprintf(buff, "INVALID_CONTEXT");
		break;
	case CL_INVALID_KERNEL_ARGS:
		sprintf(buff, "INVALID_KERNEL_ARGS");
		break;
	case CL_INVALID_WORK_DIMENSION:
		sprintf(buff, "INVALID_WORK_DIMENSION");
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		sprintf(buff, "INVALID_GLOBAL_WORK_SIZE");
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		sprintf(buff, "INVALID_GLOBAL_OFFSET");
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		sprintf(buff, "INVALID_WORK_GROUP_SIZE");
		break;
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:
		sprintf(buff, "MISALIGNED_SUB_BUFFER_OFFSET");
		break;
	case CL_INVALID_IMAGE_SIZE:
		sprintf(buff, "INVALID_IMAGE_SIZE");
		break;
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		sprintf(buff, "IMAGE_FORMAT_NOT_SUPPORTED");
		break;
	case CL_OUT_OF_RESOURCES:
		sprintf(buff, "OUT_OF_RESOURCES");
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		sprintf(buff, "MEM_OBJECT_ALLOCATION_FAILURE");
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		sprintf(buff, "INVALID_EVENT_WAIT_LIST");
		break;
	case CL_INVALID_OPERATION:
		sprintf(buff, "INVALID_OPERATION");
		break;
	case CL_OUT_OF_HOST_MEMORY:
		sprintf(buff, "OUT_OF_HOST_MEMORY");
		break;
	default:
		sprintf(buff, "NO_ERROR_CODE");
		break;
	}

	err("Failed to run kernel: %s", buff);
}

static k_Image *_CLImage_to_k_Image(CLImage clImage) {
	k_Image *image = k_create_image(clImage.size.x, clImage.size.y);

	for (int i = 0; i < image->width * image->height; i++) {
		if (clImage.data[i].x < 0)
			clImage.data[i].x = 0;
		if (clImage.data[i].y < 0)
			clImage.data[i].y = 0;
		if (clImage.data[i].z < 0)
			clImage.data[i].z = 0;

		if (clImage.data[i].x > 1)
			clImage.data[i].x = 1;
		if (clImage.data[i].y > 1)
			clImage.data[i].y = 1;
		if (clImage.data[i].z > 1)
			clImage.data[i].z = 1;

		image->data[i * 3 + 0] = (int)(clImage.data[i].x * 255);
		image->data[i * 3 + 1] = (int)(clImage.data[i].y * 255);
		image->data[i * 3 + 2] = (int)(clImage.data[i].z * 255);
	}

	return image;
}

void _set_kernel_arg(cl_uint idx, size_t size, void *arg) {
	clSetKernelArg(r.program.kernel, idx, size, arg);
}

static cl_mem _create_buffer(size_t size, cl_mem_flags flags) {
	cl_mem ret = clCreateBuffer(r.program.context, flags, size, NULL, NULL);
	return ret;
}

static cl_int _run_kernel(size_t size) {
	return clEnqueueNDRangeKernel(r.program.queue, r.program.kernel, 1, NULL,
								  &size, NULL, 0, NULL, NULL);
}

static void _setup_renderer_args() {
	int imageSize = r.image.size.x * r.image.size.y;

	safe_clReleaseMemObject(r.program.imageBuff);
	r.program.imageBuff =
		_create_buffer(sizeof(cl_float3) * imageSize, CL_MEM_READ_WRITE);

	safe_clReleaseMemObject(r.program.voxelBuff);
	r.program.voxelBuff =
		_create_buffer(sizeof(Voxel) * r.scene.voxelCount, CL_MEM_READ_ONLY);

	clEnqueueWriteBuffer(r.program.queue, r.program.voxelBuff, CL_TRUE, 0,
						 sizeof(Voxel) * r.scene.voxelCount, r.scene.voxels, 0,
						 NULL, NULL);

	safe_clReleaseMemObject(r.program.chunkBuff);
	r.program.chunkBuff =
		_create_buffer(sizeof(Chunk) * r.scene.chunkCount, CL_MEM_READ_ONLY);

	clEnqueueWriteBuffer(r.program.queue, r.program.chunkBuff, CL_TRUE, 0,
						 sizeof(Chunk) * r.scene.chunkCount, r.scene.chunks, 0,
						 NULL, NULL);

	_set_kernel_arg(0, sizeof(cl_int2), &r.image.size);
	_set_kernel_arg(1, sizeof(cl_mem), &r.program.imageBuff);
	_set_kernel_arg(2, sizeof(cl_int), &r.scene.voxelCount);
	_set_kernel_arg(3, sizeof(cl_mem), &r.program.voxelBuff);
	_set_kernel_arg(4, sizeof(cl_int), &r.scene.chunkSize);
	_set_kernel_arg(5, sizeof(cl_int), &r.scene.chunkCount);
	_set_kernel_arg(6, sizeof(cl_mem), &r.program.chunkBuff);
	_set_kernel_arg(7, sizeof(cl_float3), &r.scene.bgColor);
	_set_kernel_arg(8, sizeof(cl_float3), &r.scene.bgBrightness);
	_set_kernel_arg(9, sizeof(Camera), &r.camera);
}

static RendererStatus _render_frame(int sampleNumber) {
	cl_ulong seed = rand();
	cl_int preview = !sampleNumber;

	// non-constant arguments
	_set_kernel_arg(10, sizeof(cl_int), &sampleNumber);
	_set_kernel_arg(11, sizeof(cl_ulong), &seed);
	_set_kernel_arg(12, sizeof(cl_int), &preview);

	cl_int ret = _run_kernel(r.image.size.x * r.image.size.y);

	if (ret != CL_SUCCESS) {
		_print_kernel_run_error(ret);
		return RENDERER_FAILURE;
	}

	clEnqueueReadBuffer(r.program.queue, r.program.imageBuff, CL_TRUE, 0,
						sizeof(cl_float3) * r.image.size.x * r.image.size.y,
						r.image.data, 0, NULL, NULL);

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

		if (!r.readFirstFrame)
			i = 0;

		if (_render_frame(i) == RENDERER_FAILURE)
			panic("Failed to run kernel");

		i++;
	}

	r.stopRender = 0;

	pthread_exit(NULL);
}

//---- public ----------------------------------------------------------------//

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

	// TODO: check platform count
	cl_uint platformNum;
	clGetPlatformIDs(1, &r.program.platform, &platformNum);
	cl_uint deviceNum;
	clGetDeviceIDs(r.program.platform, CL_DEVICE_TYPE_GPU, 2, &r.program.device,
				   &deviceNum);

	r.program.context =
		clCreateContext(0, 1, &r.program.device, NULL, NULL, NULL);
	r.program.queue = clCreateCommandQueueWithProperties(
		r.program.context, r.program.device, 0, NULL);

	char *source = read_file(FILE_NAME);

	if (source == NULL) {
		_print_source_read_error();
		destroy_renderer();
		return RENDERER_FAILURE;
	}

	r.program.program = clCreateProgramWithSource(
		r.program.context, 1, (const char **)&source, NULL, NULL);

	free(source);

	if (clBuildProgram(r.program.program, 0, NULL, ARGS, NULL, NULL) !=
		CL_SUCCESS) {
		_print_program_build_error(r.program.device, r.program.program);
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
	safe_clReleaseMemObject(r.program.voxelBuff);
	safe_clReleaseMemObject(r.program.imageBuff);
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