#ifndef K_OPENCL_UTIL_H
#define K_OPENCL_UTIL_H

#include <CL/cl.h>

void safe_clReleaseMemObject(cl_mem mem);

#ifdef K_OPENCL_UTIL_IMPLEMENTATION

void safe_clReleaseMemObject(cl_mem mem) {
	if(mem != NULL) {
		clReleaseMemObject(mem);
		mem = NULL;
	}
}

#endif

#endif
