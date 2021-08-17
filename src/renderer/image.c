#include "renderer.h"

//---- private ---------------------------------------------------------------//

static void _print_kernel_run_error(cl_int ret) {
	msg("Failed to run kernel: ");

	switch (ret) {
	case CL_INVALID_PROGRAM_EXECUTABLE:
		msg("INVALID_PROGRAM_EXECUTABLE\n");
		break;
	case CL_INVALID_COMMAND_QUEUE:
		msg("INVALID_COMMAND_QUEUE\n");
		break;
	case CL_INVALID_KERNEL:
		msg("INVALID_KERNEL\n");
		break;
	case CL_INVALID_CONTEXT:
		msg("INVALID_CONTEXT\n");
		break;
	case CL_INVALID_KERNEL_ARGS:
		msg("INVALID_KERNEL_ARGS\n");
		break;
	case CL_INVALID_WORK_DIMENSION:
		msg("INVALID_WORK_DIMENSION\n");
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		msg("INVALID_GLOBAL_WORK_SIZE\n");
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		msg("INVALID_GLOBAL_OFFSET\n");
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		msg("INVALID_WORK_GROUP_SIZE\n");
		break;
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:
		msg("MISALIGNED_SUB_BUFFER_OFFSET\n");
		break;
	case CL_INVALID_IMAGE_SIZE:
		msg("INVALID_IMAGE_SIZE\n");
		break;
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		msg("IMAGE_FORMAT_NOT_SUPPORTED\n");
		break;
	case CL_OUT_OF_RESOURCES:
		msg("OUT_OF_RESOURCES\n");
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		msg("MEM_OBJECT_ALLOCATION_FAILURE\n");
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		msg("INVALID_EVENT_WAIT_LIST\n");
		break;
	case CL_INVALID_OPERATION:
		msg("INVALID_OPERATION\n");
		break;
	case CL_OUT_OF_HOST_MEMORY:
		msg("OUT_OF_HOST_MEMORY\n");
		break;
	default:
		msg("NO_ERROR_CODE\n");
		break;
	}
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

static void *_start_renderer_loop() {
	r.restartRender = 1;

	int i;
	while (!r.stopRender) {
		if (r.restartRender) {
			r.restartRender = 0;
			i = 0;
			_setup_renderer_args();
		}

		if (_render_frame(i) == RENDERER_FAILURE) {
			exit(-1);
		}
		i++;
	}

	r.stopRender = 0;

	pthread_exit(NULL);
}

//---- public ----------------------------------------------------------------//

RendererStatus set_output_properties(int width, int height) {
	safe_free(r.image.data);
	r.image.size = (cl_int2){.x = width, .y = height};
	r.image.data = malloc(sizeof(cl_float3) * width * height);

	return RENDERER_SUCCESS;
}

RendererStatus begin_rendering() {
	r.restartRender = 1;
	r.stopRender = 0;

	pthread_t thread;
	pthread_create(&thread, NULL, _start_renderer_loop, NULL);

	return RENDERER_SUCCESS;
}

RendererStatus end_rendering() {
	r.stopRender = 1;
	return RENDERER_SUCCESS;
}

k_Image *get_image() {
	// msg("(%f, %f, %f)\n", r.image.data[0].x, r.image.data[0].y,
	// 	r.image.data[0].z);
	k_Image *image = _CLImage_to_k_Image(r.image);
	k_gamma_correct_image(image);
	return image;
}