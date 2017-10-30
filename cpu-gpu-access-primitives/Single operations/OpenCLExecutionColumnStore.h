//#pragma once
#include "CL/cl.h"
#include<iostream>
#include<vector>
#include "Column_Store_Data.h"
#include<fstream>

using namespace std;

class OpenCLExecutionColumnStore
{
public:
	OpenCLExecutionColumnStore();
	~OpenCLExecutionColumnStore();
	void CheckError(cl_int error);
	cl_program CreateProgram(const std::string& source, cl_context context);
	std::string LoadKernel(const char* name);
	double InsertColumnStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, InputDataInsertColumnStore inputData, bool includeTransferTime);
	double UpdateColumnStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime);
	double SelectColumnStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime);
        double InsertColumnStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, InputDataInsertColumnStore inputData, bool includeTransferTime);
	double UpdateColumnStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime);
	double SelectColumnStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
            cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime);
	double NewOrderColumnStoreGPU(cl_context GPUcontext, cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds,
	            int queryCount, InputDataNewOrderColumnStore inputData, bool includeTransferTime);
	double NewOrderColumnStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds,
	            int queryCount, InputDataNewOrderColumnStore inputData, bool includeTransferTime);
};

