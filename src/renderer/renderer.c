#include "renderer.h"

#define FILE_NAME "data/kernel/pathtracer.cl"
#define KERNEL_NAME "pathtracer"
#define ARGS "-Werror -cl-mad-enable -cl-no-signed-zeros -cl-fast-relaxed-math"
#define CHUNK_SIZE 3

//---- private  --------------------------------------------------------------//

static void _print_source_read_error() { msg("Failed to read source file\n"); }

static void _print_program_build_error(cl_device_id device,
									   cl_program program) {
	msg("Failed to build program\n");

	char buffer[0x100000];
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer),
						  buffer, NULL);
	msg("%s\n", buffer);
}

//---- public ----------------------------------------------------------------//

RendererStatus create_renderer() {
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
		return RENDERER_FAILURE;
	}

	r.program.program = clCreateProgramWithSource(
		r.program.context, 1, (const char **)&source, NULL, NULL);

	free(source);

	if (clBuildProgram(r.program.program, 0, NULL, ARGS, NULL, NULL) !=
		CL_SUCCESS) {
		_print_program_build_error(r.program.device, r.program.program);
		return RENDERER_FAILURE;
	}

	r.program.kernel = clCreateKernel(r.program.program, KERNEL_NAME, NULL);

	return RENDERER_SUCCESS;
}

RendererStatus destroy_renderer() {
	safe_free(r.scene.voxels);
	safe_free(r.image.data);
	safe_clReleaseMemObject(r.program.voxelBuff);
	safe_clReleaseMemObject(r.program.imageBuff);
	clReleaseProgram(r.program.program);
	clReleaseKernel(r.program.kernel);
	clReleaseContext(r.program.context);

	return RENDERER_SUCCESS;
}
