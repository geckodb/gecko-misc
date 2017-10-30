
#include "OpenCLExecutionRowStore.h"
#include <math.h>
#include "CL/cl.h"
#include <chrono>

using namespace std;

extern int customerDataSize;

int customer_offsets[] = {0, 4, 6, 8, 12, 16, 36, 56,58,62,66,68,70,90,110,130,132, 141,161, 171,173,203};

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
    float N = (float)203/21;
    for (int i = 0; i < 21; i++)
        bytes[i] = N * i;
    bytes[21] = 203;

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
    

    size_t globalWorkSize[] = {21, queryCount};
    
    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel, 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    clFinish(queueGPU);

    CheckError(clEnqueueReadBuffer(queueGPU, output_buffer, CL_TRUE, 0, customerSize() * queryCount, Customers, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    
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
    
    if (!includeTransferTime) return kernelTime/1000000.0;
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

    cl_mem input_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount, inputData, &error);
    cl_mem output_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount, Customers, &error);
    cl_mem offsets = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*22, customer_offsets, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
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
    
    size_t globalWorkSize[] = {queryCount};
    
    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel, 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    clFinish(queueCPU);

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
    else return timeInSeconds/1000;;

}


double OpenCLExecutionRowStore::UpdateRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char* loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing update for row store on GPU" << std::endl;
    
    cl_int error = CL_SUCCESS;

    cl_program program_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/update_single_value.cl"), GPUcontext);
    CheckError(clBuildProgram(program_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    

    cl_kernel kernel = clCreateKernel(program_update, "update_single_value", &error);

    cl_mem index = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem data = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem offsets = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*22, customer_offsets, &error);
    char types[] = {'i', 'i', 'i', 'f', 'f', 'c', 'c', 'c', 'f', 'f', 'i', 'i', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'};
    cl_mem var_types = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*21, &types, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
    int f = 0;
    cl_mem ftest = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float), &f, &error);
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
    
    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {21, queryCount};

    unsigned long kernelTime = 0;
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel, 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;
    
    clFinish(queueGPU);

    CheckError(clEnqueueReadBuffer(queueGPU, data, CL_TRUE, 0, customerDataSize * customerSize(), loadedData, 0, nullptr, nullptr));
    //CheckError(clEnqueueReadBuffer(queueGPU, tmp, CL_TRUE, 0, sizeof(int), &itmp, 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ftest, CL_TRUE, 0, sizeof(float), &f, 0, nullptr, nullptr));
    
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
    else return timeInSeconds/1000;;
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

    cl_mem index = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem output_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem offsets = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*22, customer_offsets, &error);
    char types[] = {'i', 'i', 'i', 'f', 'f', 'c', 'c', 'c', 'f', 'f', 'i', 'i', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c'};
    cl_mem var_types = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*21, &types, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);


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
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    clFinish(queueCPU);

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
    else return timeInSeconds/1000;;
}
/*
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

    size_t globalWorkSize[] = {21, queryCount};
    
    unsigned long kernelTime = 0;
    
    
    cl_event event;
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel, 2, nullptr, globalWorkSize, nullptr, 0, nullptr, &event));
    clWaitForEvents(1, &event);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;
    
    clFinish(queueGPU);
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
    
    long long timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    free(Customers);

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;;
} */

double OpenCLExecutionRowStore::SelectRowStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
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
    else return timeInSeconds/1000;;
}

double OpenCLExecutionRowStore::SelectRowStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, cl_uint GPUdeviceIdCount, 
        std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, char* loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing select for row store on CPU" << std::endl;
    
    char* Customers = (char*) malloc(customerSize() * queryCount);
    
    cl_int error;

    cl_program program_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_select_Customers.cl"), CPUcontext);
    CheckError(clBuildProgram(program_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    
    
    cl_kernel kernel = clCreateKernel(program_select, "op_select_Customers", &error);
    

    cl_mem index = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * queryCount, inputData, &error);
    cl_mem input_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerDataSize * customerSize(), loadedData, &error);
    cl_mem output_buffer = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, customerSize() * queryCount, Customers, &error);
    cl_mem offsets = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*22, customer_offsets, &error);
    int attr = 21;
    cl_mem attributes = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int), &attr, &error);
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
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    kernelTime = end - start;

    clFinish(queueCPU);

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
