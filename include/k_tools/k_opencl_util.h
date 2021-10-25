//====================================================================================================================//
// k_opencl_util.h                                                                                                    //
//====================================================================================================================//

//--------------------------------------------------------------------------------------------------------------------//
// interface                                                                                                          //
//--------------------------------------------------------------------------------------------------------------------//

#ifndef K_OPENCL_UTIL_H
#define K_OPENCL_UTIL_H

#include <CL/cl.h>

void cl_print_source_read_error();
void cl_print_program_build_error(cl_device_id device, cl_program program);
void cl_print_kernel_run_error(cl_int ret);
void cl_destroy_buffer(cl_mem mem);
void cl_set_kernel_arg(cl_kernel kernel, cl_uint idx, size_t size, void *arg);
cl_mem cl_create_buffer(cl_context context, size_t size, cl_mem_flags flags);
cl_int cl_run_kernel(cl_command_queue queue, cl_kernel kernel, size_t size);
void cl_read_buffer(cl_command_queue queue, cl_mem mem, size_t offset, size_t size, void *ptr);

//--------------------------------------------------------------------------------------------------------------------//
// implementation                                                                                                     //
//--------------------------------------------------------------------------------------------------------------------//

#ifdef K_OPENCL_UTIL_IMPLEMENTATION

void cl_print_source_read_error() {
	err("Failed to read source file");
}

void cl_print_program_build_error(cl_device_id device, cl_program program) {
	char buff[0x100000];
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buff), buff, NULL);
	err("Failed to build program: %s", buff);
}

void cl_print_kernel_run_error(cl_int ret) {
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

void cl_destroy_buffer(cl_mem mem) {
	if (mem != NULL) {
		clReleaseMemObject(mem);
		mem = NULL;
	}
}

void cl_set_kernel_arg(cl_kernel kernel, cl_uint idx, size_t size, void *arg) {
	clSetKernelArg(kernel, idx, size, arg);
}

cl_mem cl_create_buffer(cl_context context, size_t size, cl_mem_flags flags) {
	cl_mem ret = clCreateBuffer(context, flags, size, NULL, NULL);
	return ret;
}

cl_int cl_run_kernel(cl_command_queue queue, cl_kernel kernel, size_t size) {
	return clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &size, NULL, 0, NULL, NULL);
}

void cl_read_buffer(cl_command_queue queue, cl_mem mem, size_t offset, size_t size, void *ptr) {
	clEnqueueReadBuffer(queue, mem, CL_TRUE, offset, size, ptr, 0, NULL, NULL);
}

#endif

#endif
