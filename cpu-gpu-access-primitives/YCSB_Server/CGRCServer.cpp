/*
 * CGRCServer.cpp
 *
 *  Created on: Oct 22, 2017
 *      Author: arefeva
 */

#include "CGRCServer.h"
#include <vector>

int tuple_size = 1255;

CGRCServer::CGRCServer(char device, char storage) {

	Initialize();

	cl_uint device_count;
	value_type *device_ids_data;

	if (device == 'c') {
		device_count = CPU_count;
		device_ids_data = CPUdeviceIds.data();
	}
	else {
		device_count = GPU_count;
		device_ids_data = GPUdeviceIds.data();
	}

	program_insert_rs = CreateProgram(LoadKernel("/home/arefeva/YSCB/insert_value_rs.cl"), context);
	clBuildProgram(program_insert_rs, device_count, device_ids_data, nullptr, nullptr, nullptr);
	cl_kernel kernel_insert_rs = clCreateKernel(program_insert_rs, "insert_value", nullptr);

	program_update_rs = CreateProgram(LoadKernel("/home/arefeva/YSCB/update_value_rs.cl"), context);
	clBuildProgram(program_update_rs, device_count, device_ids_data, nullptr, nullptr, nullptr);
	cl_kernel kernel_update_rs = clCreateKernel(program_update_rs, "update_value", nullptr);

	program_read_rs = CreateProgram(LoadKernel("/home/arefeva/YSCB/read_value_rs.cl"), context);
	clBuildProgram(program_read_rs, device_count, device_ids_data, nullptr, nullptr, nullptr);
	cl_kernel kernel_read_rs = clCreateKernel(program_read_rs, "read_value", nullptr);

	program_insert_cs = CreateProgram(LoadKernel("/home/arefeva/YSCB/insert_value_cs.cl"), context);
	clBuildProgram(program_insert_cs, device_count, device_ids_data, nullptr, nullptr, nullptr);
	cl_kernel kernel_insert_cs = clCreateKernel(program_insert_cs, "insert_value", nullptr);

	program_update_cs = CreateProgram(LoadKernel("/home/arefeva/YSCB/update_value_cs.cl"), context);
	clBuildProgram(program_update_cs, device_count, device_ids_data, nullptr, nullptr, nullptr);
	cl_kernel kernel_update_cs = clCreateKernel(program_update_cs, "update_value", nullptr);

	program_read_cs = CreateProgram(LoadKernel("/home/arefeva/YSCB/read_value_cs.cl"), context);
	clBuildProgram(program_read_cs, device_count, device_ids_data, nullptr, nullptr, nullptr);
	cl_kernel kernel_read_cs = clCreateKernel(program_read_cs, "read_value", nullptr);

}

CGRCServer::~CGRCServer() {

	clReleaseKernel(kernel_insert_rs);
	clReleaseKernel(kernel_insert_cs);
	clReleaseKernel(kernel_update_rs);
	clReleaseKernel(kernel_update_cs);
	clReleaseKernel(kernel_read_rs);
	clReleaseKernel(kernel_read_cs);

	clReleaseProgram(program_insert_rs);
	clReleaseProgram(program_insert_cs);
	clReleaseProgram(program_update_rs);
	clReleaseProgram(program_update_cs);
	clReleaseProgram(program_read_rs);
	clReleaseProgram(program_read_cs);

}

std::string CGRCServer::GetPlatformName(cl_platform_id id) {
	size_t size = 0;
	clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, NULL, &size);

	std::string result;
	result.resize(size);
	clGetPlatformInfo(id, CL_PLATFORM_NAME, size,
			const_cast<char*> (result.data()), NULL);

	return result;
}

std::string CGRCServer::GetDeviceName(cl_device_id id) {
	size_t size = 0;
	clGetDeviceInfo(id, CL_DEVICE_NAME, 0, NULL, &size);

	std::string result;
	result.resize(size);
	clGetDeviceInfo(id, CL_DEVICE_NAME, size, const_cast<char*> (result.data()), NULL);

	return result;
}

int CGRCServer::Initialize() {


	clGetPlatformIDs(0, NULL, &platform_count);

	if (platform_count == 0) {
		std::cerr << "No OpenCL platform found" << std::endl;
		return 1;
	}
	else {
		std::cout << "Found " << platform_count << " platform(s)" << std::endl;
	}

	std::vector<cl_platform_id> platformIds(platform_count);
	clGetPlatformIDs(platform_count, platformIds.data(), NULL);

	for (cl_uint i = 0; i < platformIdCount; ++i) {
		std::cout << "\t (" << (i + 1) << ") : " << GetPlatformName(platformIds[i]) << std::endl;
	}

	clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, 0, NULL, &GPU_count);

	if (GPUdeviceIdCount == 0) {
		std::cerr << "No GPU devices found" << std::endl;
		return 1;
	} else {
		std::cout << "Found " << GPU_count << " device(s)" << std::endl;
	}

	clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, GPU_count, GPUdeviceIds.data(), NULL);

	for (cl_uint i = 0; i < GPU_count; ++i) {
		std::cout << "\t (" << (i + 1) << ") : " << GetDeviceName(GPUdeviceIds[i]) << std::endl;
	}

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateContext.html
	const cl_context_properties GPUcontextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (platformIds[0]),
			0, 0
	};

	cl_int error = CL_SUCCESS;
	GPU_context = clCreateContext(GPUcontextProperties, GPU_count, GPUdeviceIds.data(), NULL, NULL, &error);
	CheckError(error);

	std::cout << "GPU context created" << std::endl;


	clGetDeviceIDs(platformIds[1], CL_DEVICE_TYPE_ALL, 0, NULL, &CPU_count);

	if (CPU_count == 0) {
		std::cerr << "No CPU devices found" << std::endl;
		return 1;
	} else {
		std::cout << "Found " << CPUdeviceIdCount << " device(s)" << std::endl;
	}

	clGetDeviceIDs(platformIds[1], CL_DEVICE_TYPE_ALL, CPU_count, CPUdeviceIds.data(), NULL);

	for (cl_uint i = 0; i < CPUdeviceIdCount; ++i) {
		std::cout << "\t (" << (i + 1) << ") : " << GetDeviceName(CPUdeviceIds[i]) << std::endl;
	}

	// http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateContext.html
	const cl_context_properties CPUcontextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (platformIds[1]),
			0, 0
	};

	CPU_context = clCreateContext(CPUcontextProperties, CPUdeviceIdCount, CPUdeviceIds.data(), NULL, NULL, &error);
	CheckError(error);

	std::cout << "CPU context created" << std::endl;
	return 0;

}
