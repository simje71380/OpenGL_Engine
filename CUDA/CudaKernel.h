#pragma once

#ifndef EXP_CUDA_KERNEL

	#ifndef BUILD_CUDA_KERNEL
		#pragma comment(lib, "CudaKernel.lib")
		#define EXP_CUDA_KERNEL __declspec(dllimport)
	#else
		#define EXP_CUDA_KERNEL __declspec(dllexport)
	#endif
#endif

extern "C" EXP_CUDA_KERNEL bool compute_cuda(int* c, const int* a, const int* b, int arraySize);