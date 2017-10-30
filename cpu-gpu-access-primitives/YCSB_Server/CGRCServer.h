/*
 * CGRCServer.h
 *
 *  Created on: Oct 22, 2017
 *      Author: arefeva
 */

#ifndef CGRCSERVER_H_
#define CGRCSERVER_H_

#include <string>
#include <vector>
#include "CL/cl.h"
#include "QueryExecutor.h"

long table_size = 0;

class CGRCServer {

	CGRCServer(char device, char storage);

	~CGRCServer();

public:

	cl_program program_insert_rs;
	cl_program program_insert_cs;
	cl_program program_update_rs;
	cl_program program_update_cs;
	cl_program program_read_rs;
	cl_program program_read_cs;

	cl_kernel kernel_insert_rs;
	cl_kernel kernel_insert_cs;
	cl_kernel kernel_update_rs;
	cl_kernel kernel_update_cs;
	cl_kernel kernel_read_rs;
	cl_kernel kernel_read_cs;

	cl_uint platform_count = 0;

	cl_context GPU_context;
	cl_uint GPU_count = 0;

	cl_context CPU_context;
	cl_uint CPU_count = 0;

	std::vector<cl_device_id> GPUDeviceIds;
	std::vector<cl_device_id> CPUDeviceIds;

	std::string GetPlatformName(cl_platform_id id);

	std::string GetDeviceName(cl_device_id id);

	QueryExecutor queryExecutor;

	void CheckError(cl_int error) {
	    if (error != CL_SUCCESS) {
	        std::cerr << "OpenCL call failed with error " << error << std::endl;
	        std::exit(1);
	    }
	}

	int Initialize();

};

#endif /* CGRCSERVER_H_ */
