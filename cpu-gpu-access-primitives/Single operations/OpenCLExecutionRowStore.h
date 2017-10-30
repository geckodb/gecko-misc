//#pragma once
#include "CL/cl.h"
#include<iostream>
#include<vector>
#include "Row_Store_Data.h"
#include <time.h>
#include<fstream>

using namespace std;

#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable

class OpenCLExecutionRowStore
{
public:
	OpenCLExecutionRowStore();
	~OpenCLExecutionRowStore();
	
	std::string LoadKernel(const char* name);
	void CheckError(cl_int error);
	cl_program CreateProgram(const std::string& source, cl_context context);
	double InsertRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, char* inputData, bool includeTransferTime);
	double UpdateRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char *loadedData, bool includeTransferTime);
	double SelectRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char *loadedData, bool includeTransferTime);
        double InsertRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, char* inputData, bool includeTransferTime);
	double UpdateRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char *loadedData, bool includeTransferTime);
	double SelectRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char *loadedData, bool includeTransferTime);
	double NewOrderRowStoreGPU(cl_context GPUcontext, cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds,
	            int queryCount, InputDataNewOrderRowStore inputData, bool includeTransferTime);
	double NewOrderRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds,
	            int queryCount, InputDataNewOrderRowStore inputData, bool includeTransferTime);
	string CurrentDateTime();
	
};

