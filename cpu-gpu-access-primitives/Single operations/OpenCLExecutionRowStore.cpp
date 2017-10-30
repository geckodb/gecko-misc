
#include "OpenCLExecutionRowStore.h"
#include <math.h>
#include "CL/cl.h"
#include <chrono>

using namespace std;

extern int customerDataSize;

/*struct Customer
{
	int c_id;
	int c_d_id;
	int c_w_id;
	int c_payment_cnt;
	int c_delivery_cnt;
	float c_balance;
	float c_ytd_payment;
	float c_discount;
	float c_credit_lim;
	char c_last[20];
	char c_first[20];
	char c_credit[2];
	char c_street_1[20];
	char c_street_2[20];
	char c_city[20];
	char c_state[2];
	char c_zip[9];
	char c_phone[20];
	char c_since[10];
	char c_middle[2];
	char c_data[30];
};*/

int customer_offsets[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 56, 76, 78, 98, 118, 138, 140, 149, 169, 179, 181, 211, 212};

OpenCLExecutionRowStore::OpenCLExecutionRowStore() {
}

OpenCLExecutionRowStore::~OpenCLExecutionRowStore() {
}

void OpenCLExecutionRowStore::CheckError(cl_int error) {
    if (error != CL_SUCCESS) {
        std::cerr << "OpenCL call failed with error " << error << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

std::string OpenCLExecutionRowStore::LoadKernel(const char* name) {
    std::ifstream in(name);
    std::string result(
            (std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());
    return result;
}

cl_program OpenCLExecutionRowStore::CreateProgram(const std::string& source, cl_context context) {
    size_t lengths[1] = {source.size()};
    const char* sources[1] = {source.data()};

    cl_int error = 0;
    cl_program program = clCreateProgramWithSource(context, 1, sources, lengths, &error);
    

    return program;
}

string OpenCLExecutionRowStore::OpenCLExecutionRowStore::CurrentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof (buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

double OpenCLExecutionRowStore::InsertRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, char* inputData, bool includeTransferTime) {

    //std::cout << "Executing insert for row store on GPU" << std::endl;

    char* Customers = (char*) malloc(customerSize() * queryCount);
    
    cl_int error = CL_SUCCESS;
    cl_program program_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/insert_single_value.cl"), GPUcontext);
    CheckError(clBuildProgram(program_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    
    int bytes[22];
    float N = (float)211/21;
    for (int i = 0; i < 21; i++)
        bytes[i] = N * i;
    bytes[21] = 211;

    cl_kernel kernel = clCreateKernel(program_insert, "insert_single_value", &error);
    
    cl_mem input_buffer = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount, inputData, &error);
    cl_mem output_buffer = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount, Customers, &error);
    cl_mem offsets = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*22, bytes, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
    
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &attributes);
    //clSetKernelArg(kernel, 3, sizeof (cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);
    
    //cl_char* dataIn = (cl_char*)clEnqueueMapBuffer(queueGPU, input_buffer, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, customerSize() * queryCount, 0, nullptr, nullptr, NULL);
    //cl_char* dataOut = (cl_char*)clEnqueueMapBuffer(queueGPU, output_buffer, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, customerSize() * queryCount, 0, nullptr, nullptr, NULL);

    /*cl_event transfer1;
    clEnqueueWriteBuffer(queueGPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, &transfer1);
    //clEnqueueWriteBuffer(queueGPU, offsets, CL_TRUE, 0, sizeof(int)*22, customer_offsets, 0, nullptr, &transfer1);
    clWaitForEvents(1, &transfer1);
    unsigned long start1 = 0;
        unsigned long end1 = 0;
        clGetEventProfilingInfo(transfer1, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start1, NULL);
        clGetEventProfilingInfo(transfer1, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end1, NULL);
        unsigned long transferTime1 = end1 - start1;*/


    unsigned long start = 0;
    unsigned long end = 0;


    /*cl_event transfer;
    clEnqueueWriteBuffer (queueGPU, input_buffer, CL_TRUE, 0, customerSize() * queryCount, inputData, 0, nullptr, &transfer);
    //clEnqueueWriteBuffer(queueGPU, offsets, CL_TRUE, 0, sizeof(int)*22, customer_offsets, 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, inputData, 0, nullptr, &transfer);
    clWaitForEvents(1, &transfer);

    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;*/


    size_t globalWorkSize[] = {21, queryCount};
    
    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel, 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);

    clFinish(queueGPU);

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    cl_event transfer;

    clEnqueueReadBuffer(queueGPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, &transfer);

    clWaitForEvents(1, &transfer);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;

    //CheckError(clEnqueueReadBuffer(queueGPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));

     //CheckError(clEnqueueUnmapMemObject(queueGPU, output_buffer, dataOut, 0, nullptr, nullptr));
    // CheckError(clEnqueueUnmapMemObject(queueGPU, input_buffer, dataIn, 0, nullptr, nullptr));

    //cout << itmp << endl;

    clReleaseCommandQueue(queueGPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    //clReleaseMemObject(tmp);
    
    clReleaseKernel(kernel);
    clReleaseProgram(program_insert);
    
    
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    free(Customers);
    
    if (!includeTransferTime) return transferTime/1000000.0;
    else return timeInSeconds/1000;;
}

/*double OpenCLExecutionRowStore::InsertRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, char* inputData, bool includeTransferTime) {

    //std::cout << "Executing insert for row store on GPU" << std::endl;

    char* Customers = (char*) malloc(customerSize() * queryCount);
    
    cl_int error = CL_SUCCESS;
    cl_program program_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/insert_single_value.cl"), GPUcontext);
    CheckError(clBuildProgram(program_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    
    int bytes[22];
    float N = (float)203/21;
    for (int i = 0; i < 21; i++)
        bytes[i] = N * i;
    bytes[21] = 203;

    cl_kernel kernel[10];
    cl_mem input_buffer[10];
    cl_mem output_buffer[10];
    for (int i = 0; i < 10; i++) {
        kernel[i] = clCreateKernel(program_insert, "insert_single_value", &error);
        input_buffer[i] = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount/10, inputData + ((queryCount/10)*(customer_offsets[21]*i)), &error);
        output_buffer[i] = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount/10, Customers + ((queryCount/10)*(customer_offsets[21]*i)), &error);
    }
    
    cl_mem offsets = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*22, bytes, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
    
    for (int i = 0; i < 10; i++) {
    clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &input_buffer[i]);
    clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &output_buffer[i]);
    clSetKernelArg(kernel[i], 2, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel[i], 3, sizeof(cl_mem), &attributes);
    }
    
    
    //clSetKernelArg(kernel, 3, sizeof (cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);
    

    size_t globalWorkSize[] = {21, queryCount/10};
    
    unsigned long kernelTime = 0;
    
    cl_event event;
    for (int i = 0; i < 10; i++) {
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[i], 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    }
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    clFinish(queueGPU);

    for (int i = 0; i < 10; i++) {
    CheckError(clEnqueueReadBuffer(queueGPU, output_buffer[i], CL_TRUE, 0, customerSize() * queryCount/10, Customers + ((queryCount/10)*(customer_offsets[21]*i)), 0, nullptr, nullptr));
    }
    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    
    //cout << itmp << endl;
    
    clReleaseCommandQueue(queueGPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    for (int i = 0; i < 10; i++) {
    clReleaseMemObject(input_buffer[i]);
    clReleaseMemObject(output_buffer[i]);
    
    //clReleaseMemObject(tmp);
    
    clReleaseKernel(kernel[i]);
    }
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    clReleaseProgram(program_insert);
    
    
    long long timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    free(Customers);
    
    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;;
}*/

double OpenCLExecutionRowStore::InsertRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, char* inputData, bool includeTransferTime) {

    //std::cout << "Executing insert for row store on CPU" << std::endl;
    
    char* Customers = (char*) malloc(customerSize() * queryCount);
    
    cl_int error = CL_SUCCESS;
    cl_program program_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_insert_Customers.cl"), CPUcontext);
    CheckError(clBuildProgram(program_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;

    cl_mem input_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, customerSize() * queryCount, inputData, &error);
    cl_mem output_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, customerSize() * queryCount, Customers, &error);
    cl_mem offsets = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*22, customer_offsets, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &attr, &error);
    //int itmp = 10; //just to check if the kernel is executed the right number of times
    //cl_mem tmp = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &itmp, &error);
    
    auto startTime = std::chrono::high_resolution_clock::now();

    cl_kernel kernel = clCreateKernel(program_insert, "op_insert_Customers", &error);

    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);
    
    clSetKernelArg(kernel, 0, sizeof (cl_mem), &input_buffer);
    clSetKernelArg(kernel, 1, sizeof (cl_mem), &output_buffer);
    clSetKernelArg(kernel, 2, sizeof (cl_mem), &offsets);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &attributes);
    //clSetKernelArg(kernel, 3, sizeof(cl_mem), &tmp);
    

    /*cl_event transfer;
    clEnqueueWriteBuffer (queueCPU, input_buffer, CL_TRUE, 0, customerSize() * queryCount, inputData, 0, nullptr, &transfer);
    clWaitForEvents(1, &transfer);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;*/

    size_t globalWorkSize[] = {queryCount};
    
    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel, 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;


    clFinish(queueCPU);

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    CheckError(clEnqueueReadBuffer(queueCPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, nullptr));
   // CheckError(clEnqueueReadBuffer(queueCPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    
    //cout << itmp << endl;

    clReleaseCommandQueue(queueCPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(output_buffer);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    //clReleaseMemObject(tmp);
    
    clReleaseKernel(kernel);
    clReleaseProgram(program_insert);
    
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    free(Customers);
    
    //cout << "time taken" << double(timeInSeconds) << endl;

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;

}


double OpenCLExecutionRowStore::UpdateRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char* loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing update for row store on GPU" << std::endl;
    
    cl_int error = CL_SUCCESS;

    cl_program program_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/update_single_value.cl"), GPUcontext);
    CheckError(clBuildProgram(program_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    
    customerDataSize = 10000000;


    cl_kernel kernel = clCreateKernel(program_update, "update_single_value", &error);

    cl_mem index = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem data = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem offsets = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*23, customer_offsets, &error);
    char types[] = {'i', 'i', 'i', 'i', 'i', 'f', 'f', 'f', 'f', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'};
    cl_mem var_types = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*21, &types, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
    //int f = 0;
    //cl_mem ftest = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float), &f, &error);
    //int itmp = queryCount; //just to check if the kernel is executed the right number of times
    //cl_mem tmp = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &itmp, &error);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &data);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &index);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &attributes);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &var_types);
    //clSetKernelArg(kernel, 5, sizeof(cl_mem), &ftest);
    //clSetKernelArg(kernel, 4, sizeof(cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);
    unsigned long start = 0;
    unsigned long end = 0;

    unsigned long transferTime = 0;

    //cl_char* dataIn = (cl_char*)clEnqueueMapBuffer(queueGPU, index, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, customerSize() * queryCount, 0, nullptr, nullptr, NULL);
    cl_char* dataIn2 = (cl_char*)clEnqueueMapBuffer(queueGPU, data, CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, customerDataSize * customerSize(), 0, nullptr, nullptr, NULL);


    /*cl_event transfer;
    //clEnqueueWriteBuffer(queueGPU, data, CL_TRUE, 0, customerDataSize * customerSize(), loadedData, 0, nullptr, &transfer);
    //clEnqueueWriteBuffer(queueGPU, offsets, CL_TRUE, 0, sizeof(int)*22, customer_offsets, 0, nullptr, &transfer);
    //clEnqueueWriteBuffer(queueGPU, var_types, CL_TRUE, 0, sizeof(char)*21, &types, 0, nullptr, &transfer);
    clEnqueueWriteBuffer(queueGPU, index, CL_TRUE, 0, sizeof(int) * queryCount, inputData, 0, nullptr, &transfer);
    clWaitForEvents(1, &transfer);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    transferTime += end - start;*/

    //std::cout << "test1" << std::endl;

    size_t globalWorkSize[] = {queryCount};

    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel, 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);

    clFinish(queueGPU);

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    //std::cout << "test2" << std::endl;

    //cl_event transfer;

    /*clEnqueueReadBuffer(queueGPU, data, CL_TRUE, 0, customerDataSize * customerSize(), loadedData, 0, nullptr, &transfer);

    clWaitForEvents(1, &transfer);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;*/

    //CheckError(clEnqueueReadBuffer(queueGPU, data, CL_TRUE, 0, customerDataSize * customerSize(), loadedData, 0, nullptr, nullptr));

    //CheckError(clEnqueueUnmapMemObject(queueGPU, index, dataIn, 0, nullptr, nullptr));
    CheckError(clEnqueueUnmapMemObject(queueGPU, data, dataIn2, 0, nullptr, nullptr));

    //std::cout << "test3" << std::endl;

    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueGPU, ftest, CL_TRUE, 0, sizeof(float), &f, 0, nullptr, nullptr));
    
    //cout << f << endl;
    //cout << itmp << endl;

    clReleaseCommandQueue(queueGPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    clReleaseProgram(program_update);
    clReleaseKernel(kernel);
    clReleaseMemObject(data);
    clReleaseMemObject(index);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    clReleaseMemObject(var_types);
    //clReleaseMemObject(tmp);
    
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}

double OpenCLExecutionRowStore::UpdateRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char* loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing update for row store on CPU" << std::endl;
    
    cl_int error = CL_SUCCESS;

    cl_program program_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_update_Customers.cl"), CPUcontext);
    CheckError(clBuildProgram(program_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    
    cl_kernel kernel = clCreateKernel(program_update, "op_update_Customers", &error);
    //cl_kernel kernel = clCreateKernel(program_update, "update_single_value", &error);

    cl_mem index = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem output_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem offsets = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof (int)*23, customer_offsets, &error);
    char types[] = {'i', 'i', 'i', 'f', 'f', 'c', 'c', 'c', 'f', 'f', 'i', 'i', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'};
    cl_mem var_types = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(char)*21, &types, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &attr, &error);


    clSetKernelArg(kernel, 0, sizeof(cl_mem), &output_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &index);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &attributes);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &var_types);

    //clSetKernelArg(kernel, 4, sizeof(cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);


    size_t globalWorkSize[] = {queryCount};
    
    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel, 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);


    clFinish(queueCPU);

    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);

    kernelTime = end - start;

    CheckError(clEnqueueReadBuffer(queueCPU, output_buffer, CL_TRUE, 0, customerDataSize * customerSize(), loadedData, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueCPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    
    //cout << itmp << endl;
    
    
    clReleaseCommandQueue(queueCPU);
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    clReleaseProgram(program_update);
    clReleaseKernel(kernel);
    clReleaseMemObject(output_buffer);
    clReleaseMemObject(index);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    clReleaseMemObject(var_types);
    //clReleaseMemObject(tmp);
    
   /* cl_mem index = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem data = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem offsets = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*22, customer_offsets, &error);
    char types[] = {'i', 'i', 'i', 'f', 'f', 'c', 'c', 'c', 'f', 'f', 'i', 'i', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'};
    cl_mem var_types = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*21, &types, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
    int f = 0;
    cl_mem ftest = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float), &f, &error);
    //int itmp = queryCount; //just to check if the kernel is executed the right number of times
    //cl_mem tmp = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &itmp, &error);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &data);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &index);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &attributes);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &var_types);
    clSetKernelArg(kernel, 5, sizeof(cl_mem), &ftest);
    //clSetKernelArg(kernel, 4, sizeof(cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {21, queryCount};

    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel, 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;
    
    clFinish(queueCPU);

    CheckError(clEnqueueReadBuffer(queueCPU, data, CL_TRUE, 0, customerDataSize * customerSize(), loadedData, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ftest, CL_TRUE, 0, sizeof(float), &f, 0, nullptr, nullptr));
    
    cout << f << endl;
    //cout << itmp << endl;

    clReleaseCommandQueue(queueCPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    clReleaseProgram(program_update);
    clReleaseKernel(kernel);
    clReleaseMemObject(data);
    clReleaseMemObject(index);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    clReleaseMemObject(var_types);
    //clReleaseMemObject(tmp);*/
    
    
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
   // delete[] chr;
    
    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}


double OpenCLExecutionRowStore::SelectRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char* loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing select for row store on GPU" << std::endl;
    
    char* Customers = (char*) malloc(customerSize() * queryCount);
    
    cl_int error;

    cl_program program_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/select_single_value.cl"), GPUcontext);
    CheckError(clBuildProgram(program_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;

    cl_kernel kernel = clCreateKernel(program_select, "select_single_value", &error);

    cl_mem index = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem input_buffer = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem output_buffer = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount, Customers, &error);
    cl_mem offsets = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (size_t)*22, customer_offsets, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
    //int itmp = queryCount;
    //cl_mem tmp = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &itmp, &error);

    
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &index);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &attributes);
    //clSetKernelArg(kernel, 4, sizeof(cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    cl_event transfer;
    //clEnqueueWriteBuffer (queueGPU, index, CL_TRUE, 0, sizeof(int) * queryCount, inputData, 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, input_buffer, CL_TRUE, 0, customerDataSize * customerSize(), loadedData, 0, nullptr, &transfer);
    clEnqueueWriteBuffer (queueGPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, &transfer);
    clWaitForEvents(1, &transfer);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;

    size_t globalWorkSize[] = {21, queryCount};
    
    unsigned long kernelTime = 0;
    
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel, 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);

    clFinish(queueGPU);

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;
    
    CheckError(clEnqueueReadBuffer(queueGPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    
    //cout << itmp << endl;

    clReleaseCommandQueue(queueGPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseProgram(program_select);
    clReleaseKernel(kernel);

    clReleaseMemObject(input_buffer);
    clReleaseMemObject(index);
    clReleaseMemObject(output_buffer);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    free(Customers);

    if (!includeTransferTime) return transferTime/1000000.0;
    else return timeInSeconds/1000;
}

/*double OpenCLExecutionRowStore::SelectRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount,
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char* loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing select for row store on GPU" << std::endl;
    
    char* Customers = (char*) malloc(customerSize() * queryCount);
    
    cl_int error;

    cl_program program_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/select_single_value.cl"), GPUcontext);
    CheckError(clBuildProgram(program_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    
    cl_kernel kernel[10];
    cl_mem index[10];
    cl_mem output_buffer[10];
    
    for (int i = 0; i < 10; i++) {
        kernel[i] = clCreateKernel(program_select, "select_single_value", &error);
        index[i] = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * queryCount/10, inputData + (queryCount/10)*i, &error);
        output_buffer[i] = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount/10, Customers + ((queryCount/10)*(customer_offsets[21]*i)), &error);
    }

    cl_mem input_buffer = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem offsets = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*22, customer_offsets, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
    
    for (int i = 0; i < 10; i++) {
    clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &input_buffer);
    clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &output_buffer[i]);
    clSetKernelArg(kernel[i], 2, sizeof(cl_mem), &index[i]);
    clSetKernelArg(kernel[i], 3, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel[i], 4, sizeof(cl_mem), &attributes);
    }
    //clSetKernelArg(kernel, 4, sizeof(cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    cl_event transfer;
    clEnqueueWriteBuffer (queueGPU, input_buffer, CL_TRUE, 0, customerSize() * queryCount, inputData, 0, nullptr, &transfer);
    clWaitForEvents(1, &transfer);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;

    size_t globalWorkSize[] = {21, queryCount/10};
    
    unsigned long kernelTime = 0;
    
    
    cl_event event;
    for (int i = 0; i < 10; i++) {
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[i], 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    }
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;
    
    clFinish(queueGPU);

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
        clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
        kernelTime = end - start;


    for (int i = 0; i < 10; i++) {
    CheckError(clEnqueueReadBuffer(queueGPU, output_buffer[i], CL_TRUE, 0, customerSize() * queryCount/10, Customers + ((queryCount/10)*(customer_offsets[21]*i)), 0, nullptr, nullptr));
    }
    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    
    //cout << itmp << endl;

    clReleaseCommandQueue(queueGPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseProgram(program_select);
    for (int i = 0; i < 10; i++) {
    clReleaseKernel(kernel[i]);
    clReleaseMemObject(index[i]);
    clReleaseMemObject(output_buffer[i]);
    }
            
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    free(Customers);

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}*/

double OpenCLExecutionRowStore::SelectRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char* loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing select for row store on CPU" << std::endl;
    
    char* Customers = (char*) malloc(customerSize() * queryCount);
    
    cl_int error;

    cl_program program_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_select_Customers.cl"), CPUcontext);
    CheckError(clBuildProgram(program_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    
    
    cl_kernel kernel = clCreateKernel(program_select, "op_select_Customers", &error);
    

    cl_mem index = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem input_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem output_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, customerSize() * queryCount, Customers, &error);
    cl_mem offsets = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*22, customer_offsets, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &attr, &error);
    //int itmp = queryCount; //just to check if the kernel is executed the right number of times
    //cl_mem tmp = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int), &itmp, &error);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &index);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &offsets);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &attributes);
    //clSetKernelArg(kernel, 4, sizeof(cl_mem), &tmp);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {queryCount};
    
    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel, 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;


    clFinish(queueCPU);

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
        clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
        kernelTime = end - start;

    CheckError(clEnqueueReadBuffer(queueCPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueCPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    
    //cout << itmp << endl;

    clReleaseCommandQueue(queueCPU);
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    clReleaseProgram(program_select);
    clReleaseKernel(kernel);
    clReleaseMemObject(input_buffer);
    clReleaseMemObject(index);
    clReleaseMemObject(output_buffer);
    clReleaseMemObject(offsets);
    clReleaseMemObject(attributes);
    
    
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    free(Customers);

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}


double OpenCLExecutionRowStore::NewOrderRowStoreGPU(cl_context GPUcontext, cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds,
        int queryCount, InputDataNewOrderRowStore inputData, bool includeTransferTime) {

    int OLCount = inputData.total_order_lines;

    char* NewOrder = (char*) malloc(newOrderSize() * queryCount);
    char* OrderLine = (char*) malloc(orderLineSize() * OLCount);
    char* Order = (char*) malloc(ordersSize() * queryCount);

    cl_int error = CL_SUCCESS;
    cl_program program_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/insert_single_value.cl"), GPUcontext);
    CheckError(clBuildProgram(program_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    float timeInSeconds;

    int NOBytes[4];
    float N = (float)newOrderSize()/3;
    for (int i = 0; i < 3; i++)
        NOBytes[i] = N * i;
    NOBytes[3] = newOrderSize();

    int OLBytes[11];
    N = (float)orderLineSize()/10;
    for (int i = 0; i < 10; i++)
        OLBytes[i] = N * i;
    OLBytes[10] = orderLineSize();

    int OBytes[9];
    N = (float)ordersSize()/8;
    for (int i = 0; i < 8; i++)
        OBytes[i] = N * i;
    OBytes[8] = ordersSize();

    cl_kernel kernel[3];
    for (int i = 0; i < 3; i++) {
        kernel[i] = clCreateKernel(program_insert, "insert_single_value", &error);
    }

    cl_mem input_buffer_NewOrder = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, newOrderSize() * queryCount, inputData.NewOrder, &error);
    cl_mem output_buffer_NewOrder = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, newOrderSize() * queryCount, NewOrder, &error);
    cl_mem input_buffer_OrderLine = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, orderLineSize() * OLCount, inputData.OrderLine, &error);
    cl_mem output_buffer_OrderLine = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, orderLineSize() * OLCount, OrderLine, &error);
    cl_mem input_buffer_Order = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, ordersSize() * queryCount, inputData.Orders, &error);
    cl_mem output_buffer_Order = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, ordersSize() * queryCount, Order, &error);
    cl_mem NOOffsets = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*4, NOBytes, &error);
    cl_mem OLOffsets = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*11, OLBytes, &error);
    cl_mem OOffsets = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*9, OBytes, &error);
    int NOattr = 3;
    int OLattr = 10;
    int Oattr = 8;
    cl_mem NOattributes = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &NOattr, &error);
    cl_mem OLattributes = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &OLattr, &error);
    cl_mem Oattributes = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &Oattr, &error);

    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &input_buffer_NewOrder);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &output_buffer_NewOrder);
    clSetKernelArg(kernel[0], 2, sizeof(cl_mem), &NOOffsets);
    clSetKernelArg(kernel[0], 3, sizeof(cl_mem), &NOattributes);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &input_buffer_OrderLine);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &output_buffer_OrderLine);
    clSetKernelArg(kernel[1], 2, sizeof(cl_mem), &OLOffsets);
    clSetKernelArg(kernel[1], 3, sizeof(cl_mem), &OLattributes);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &input_buffer_Order);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &output_buffer_Order);
    clSetKernelArg(kernel[2], 2, sizeof(cl_mem), &OOffsets);
    clSetKernelArg(kernel[2], 3, sizeof(cl_mem), &Oattributes);

    auto startTime = std::chrono::high_resolution_clock::now();
    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSizeNO[] = {3, queryCount};
    size_t globalWorkSizeOL[] = {10, OLCount};
    size_t globalWorkSizeO[] = {8, queryCount};

    unsigned long kernelTime = 0;

    cl_event event[3];
    unsigned long start = 0;
    unsigned long end = 0;

    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[0], 2, nullptr, globalWorkSizeNO, nullptr, 0, nullptr, &event[0]));
    clWaitForEvents(1, &event[0]);
    clGetEventProfilingInfo(event[0], CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event[0], CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime += end - start;

    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[1], 2, nullptr, globalWorkSizeOL, nullptr, 0, nullptr, &event[1]));
    clWaitForEvents(1, &event[1]);
    clGetEventProfilingInfo(event[1], CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event[1], CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime += end - start;

    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[2], 2, nullptr, globalWorkSizeO, nullptr, 0, nullptr, &event[2]));
    clWaitForEvents(1, &event[2]);
    clGetEventProfilingInfo(event[2], CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event[2], CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime += end - start;

    clFinish(queueGPU);

    CheckError(clEnqueueReadBuffer(queueGPU, output_buffer_NewOrder, CL_TRUE, 0, newOrderSize() * queryCount, NewOrder, 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, output_buffer_OrderLine, CL_TRUE, 0, orderLineSize() * OLCount, OrderLine, 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, output_buffer_Order, CL_TRUE, 0, ordersSize() * queryCount, Order, 0, nullptr, nullptr));

    clReleaseCommandQueue(queueGPU);

    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseMemObject(input_buffer_NewOrder);
    clReleaseMemObject(output_buffer_NewOrder);
    clReleaseMemObject(NOOffsets);
    clReleaseMemObject(NOattributes);
    clReleaseMemObject(input_buffer_OrderLine);
    clReleaseMemObject(output_buffer_OrderLine);
    clReleaseMemObject(OLOffsets);
    clReleaseMemObject(OLattributes);
    clReleaseMemObject(input_buffer_Order);
    clReleaseMemObject(output_buffer_Order);
    clReleaseMemObject(OOffsets);
    clReleaseMemObject(Oattributes);
    //clReleaseMemObject(tmp);

    for (int i = 0; i < 3; i++) {
        clReleaseKernel(kernel[i]);
    }
    clReleaseProgram(program_insert);


    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    free(NewOrder);
    free(OrderLine);
    free(Order);

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}


double OpenCLExecutionRowStore::NewOrderRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds,
        int queryCount, InputDataNewOrderRowStore inputData, bool includeTransferTime) {

    int OLCount = inputData.total_order_lines;

    char* NewOrder = (char*) malloc(newOrderSize() * queryCount);
    char* OrderLine = (char*) malloc(orderLineSize() * OLCount);
    char* Order = (char*) malloc(ordersSize() * queryCount);

    cl_int error = CL_SUCCESS;
    cl_program program_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_insert_tuple.cl"), CPUcontext);
    CheckError(clBuildProgram(program_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    float timeInSeconds;

    int NOBytes[4];
    float N = (float)newOrderSize()/3;
    for (int i = 0; i < 3; i++)
        NOBytes[i] = N * i;
    NOBytes[3] = newOrderSize();

    int OLBytes[11];
    N = (float)orderLineSize()/10;
    for (int i = 0; i < 10; i++)
        OLBytes[i] = N * i;
    OLBytes[10] = orderLineSize();

    int OBytes[9];
    N = (float)ordersSize()/8;
    for (int i = 0; i < 8; i++)
        OBytes[i] = N * i;
    OBytes[8] = ordersSize();

    cl_kernel kernel[3];
    for (int i = 0; i < 3; i++) {
        kernel[i] = clCreateKernel(program_insert, "op_insert_tuple", &error);
    }

    cl_mem input_buffer_NewOrder = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, newOrderSize() * queryCount, inputData.NewOrder, &error);
    cl_mem output_buffer_NewOrder = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, newOrderSize() * queryCount, NewOrder, &error);
    cl_mem input_buffer_OrderLine = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, orderLineSize() * OLCount, inputData.OrderLine, &error);
    cl_mem output_buffer_OrderLine = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, orderLineSize() * OLCount, OrderLine, &error);
    cl_mem input_buffer_Order = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, ordersSize() * queryCount, inputData.Orders, &error);
    cl_mem output_buffer_Order = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, ordersSize() * queryCount, Order, &error);
    cl_mem NOOffsets = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*4, NOBytes, &error);
    cl_mem OLOffsets = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*11, OLBytes, &error);
    cl_mem OOffsets = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*9, OBytes, &error);
    int NOattr = 3;
    int OLattr = 10;
    int Oattr = 8;
    cl_mem NOattributes = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &NOattr, &error);
    cl_mem OLattributes = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &OLattr, &error);
    cl_mem Oattributes = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &Oattr, &error);

    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &input_buffer_NewOrder);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &output_buffer_NewOrder);
    clSetKernelArg(kernel[0], 2, sizeof(cl_mem), &NOOffsets);
    clSetKernelArg(kernel[0], 3, sizeof(cl_mem), &NOattributes);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &input_buffer_OrderLine);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &output_buffer_OrderLine);
    clSetKernelArg(kernel[1], 2, sizeof(cl_mem), &OLOffsets);
    clSetKernelArg(kernel[1], 3, sizeof(cl_mem), &OLattributes);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &input_buffer_Order);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &output_buffer_Order);
    clSetKernelArg(kernel[2], 2, sizeof(cl_mem), &OOffsets);
    clSetKernelArg(kernel[2], 3, sizeof(cl_mem), &Oattributes);

    auto startTime = std::chrono::high_resolution_clock::now();
    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {queryCount};
    size_t globalWorkSizeOL[] = {OLCount};

    unsigned long kernelTime = 0;

    cl_event event[3];
    unsigned long start = 0;
    unsigned long end = 0;

    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[0], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[0]));
    clWaitForEvents(1, &event[0]);
    clGetEventProfilingInfo(event[0], CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event[0], CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime += end - start;

    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[1], 1, nullptr, globalWorkSizeOL, nullptr, 0, nullptr, &event[1]));
    clWaitForEvents(1, &event[1]);
    clGetEventProfilingInfo(event[1], CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event[1], CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime += end - start;

    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[2], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[2]));
    clWaitForEvents(1, &event[2]);
    clGetEventProfilingInfo(event[2], CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event[2], CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime += end - start;

    clFinish(queueCPU);

    CheckError(clEnqueueReadBuffer(queueCPU, output_buffer_NewOrder, CL_TRUE, 0, newOrderSize() * queryCount, NewOrder, 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, output_buffer_OrderLine, CL_TRUE, 0, orderLineSize() * OLCount, OrderLine, 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, output_buffer_Order, CL_TRUE, 0, ordersSize() * queryCount, Order, 0, nullptr, nullptr));

    clReleaseCommandQueue(queueCPU);

    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseMemObject(input_buffer_NewOrder);
    clReleaseMemObject(output_buffer_NewOrder);
    clReleaseMemObject(NOOffsets);
    clReleaseMemObject(NOattributes);
    clReleaseMemObject(input_buffer_OrderLine);
    clReleaseMemObject(output_buffer_OrderLine);
    clReleaseMemObject(OLOffsets);
    clReleaseMemObject(OLattributes);
    clReleaseMemObject(input_buffer_Order);
    clReleaseMemObject(output_buffer_Order);
    clReleaseMemObject(OOffsets);
    clReleaseMemObject(Oattributes);
    //clReleaseMemObject(tmp);

    for (int i = 0; i < 3; i++) {
        clReleaseKernel(kernel[i]);
    }
    clReleaseProgram(program_insert);


    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    free(NewOrder);
    free(OrderLine);
    free(Order);

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}
