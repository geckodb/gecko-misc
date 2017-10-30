
#include "OpenCLExecutionColumnStore.h"
#include "CL/cl.h"
#include <chrono>

OpenCLExecutionColumnStore::OpenCLExecutionColumnStore() {
}

OpenCLExecutionColumnStore::~OpenCLExecutionColumnStore() {
}

using namespace std;

void OpenCLExecutionColumnStore::CheckError(cl_int error) {
    if (error != CL_SUCCESS) {
        std::cerr << "OpenCL call failed with error " << error << std::endl;
        std::exit(1);
    }
}

cl_program OpenCLExecutionColumnStore::CreateProgram(const std::string& source, cl_context context) {
    size_t lengths[1] = {source.size()};
    const char* sources[1] = {source.data()};

    cl_int error = 0;
    cl_program program = clCreateProgramWithSource(context, 1, sources, lengths, &error);
    

    return program;
}

std::string OpenCLExecutionColumnStore::LoadKernel(const char* name) {
    std::ifstream in(name);
    std::string result(
            (std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>());

    return result;
}

double OpenCLExecutionColumnStore::InsertColumnStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, InputDataInsertColumnStore inputData, bool includeTransferTime) {

    //std::cout << "Executing insert for column store on GPU" << std::endl;
    
    std::vector<int> C_ID(queryCount);
    std::vector<int> C_D_ID(queryCount);
    std::vector<int> C_W_ID(queryCount);
    std::vector<float> C_DISCOUNT(queryCount);
    std::vector<float> C_CREDIT_LIM(queryCount);
    std::vector<charArray20> C_LAST(queryCount);
    std::vector<charArray20> C_FIRST(queryCount);
    std::vector<charArray2> C_CREDIT(queryCount);
    std::vector<float> C_BALANCE(queryCount);
    std::vector<float> C_YTD_PAYMENT(queryCount);
    std::vector<int> C_PAYMENT_CNT(queryCount);
    std::vector<int> C_DELIVERY_CNT(queryCount);
    std::vector<charArray20> C_STREET_1(queryCount);
    std::vector<charArray20> C_STREET_2(queryCount);
    std::vector<charArray20> C_CITY(queryCount);
    std::vector<charArray2> C_STATE(queryCount);
    std::vector<charArray9> C_ZIP(queryCount);
    std::vector<charArray20> C_PHONE(queryCount);
    std::vector<charArray10> C_SINCE(queryCount);
    std::vector<charArray2> C_MIDDLE(queryCount);
    std::vector<charArray30> C_DATA(queryCount);

    cl_int error = CL_SUCCESS;

    cl_program GPUprogram_char2_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char2_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char9_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char9_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char10_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char10_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char20_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char20_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char30_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char30_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_int_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_int_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_float_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_float_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    //auto startTime = std::chrono::high_resolution_clock::now();

    cl_kernel kernel[21];

    for (int i = 0; i < 5; i++) {
        kernel[i] = clCreateKernel(GPUprogram_int_insert, "op_int_insert", &error);
    }
    for (int i = 5; i < 9; i++) {
        kernel[i] = clCreateKernel(GPUprogram_float_insert, "op_float_insert", &error);
    }
    kernel[9] = clCreateKernel(GPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[10] = clCreateKernel(GPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[11] = clCreateKernel(GPUprogram_char2_insert, "op_char2_insert", &error);
    kernel[12] = clCreateKernel(GPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[13] = clCreateKernel(GPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[14] = clCreateKernel(GPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[15] = clCreateKernel(GPUprogram_char2_insert, "op_char2_insert", &error);
    kernel[16] = clCreateKernel(GPUprogram_char9_insert, "op_char9_insert", &error);
    kernel[17] = clCreateKernel(GPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[18] = clCreateKernel(GPUprogram_char10_insert, "op_char10_insert", &error);
    kernel[19] = clCreateKernel(GPUprogram_char2_insert, "op_char2_insert", &error);
    kernel[20] = clCreateKernel(GPUprogram_char30_insert, "op_char30_insert", &error);
    
    cl_mem ib_1 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, inputData.C_ID.data(), &error);
    cl_mem ib_2 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, inputData.C_D_ID.data(), &error);
    cl_mem ib_3 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, inputData.C_W_ID.data(), &error);
    cl_mem ib_4 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, inputData.C_PAYMENT_CNT.data(), &error);
    cl_mem ib_5 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, inputData.C_DELIVERY_CNT.data(), &error);
    cl_mem ib_6 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, inputData.C_DISCOUNT.data(), &error);
    cl_mem ib_7 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, inputData.C_CREDIT_LIM.data(), &error);
    cl_mem ib_8 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, inputData.C_BALANCE.data(), &error);
    cl_mem ib_9 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, inputData.C_YTD_PAYMENT.data(), &error);
    cl_mem ib_10 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_LAST.data(), &error);
    cl_mem ib_11 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_FIRST.data(), &error);
    cl_mem ib_12 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * queryCount, inputData.C_CREDIT.data(), &error);
    cl_mem ib_13 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_STREET_1.data(), &error);
    cl_mem ib_14 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_STREET_2.data(), &error);
    cl_mem ib_15 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_CITY.data(), &error);
    cl_mem ib_16 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * queryCount, inputData.C_STATE.data(), &error);
    cl_mem ib_17 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray9) * queryCount, inputData.C_ZIP.data(), &error);
    cl_mem ib_18 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_PHONE.data(), &error);
    cl_mem ib_19 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray10) * queryCount, inputData.C_SINCE.data(), &error);
    cl_mem ib_20 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * queryCount, inputData.C_MIDDLE.data(), &error);
    cl_mem ib_21 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray30) * queryCount, inputData.C_DATA.data(), &error);

    cl_mem ob_1 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, C_ID.data(), &error);
    cl_mem ob_2 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, C_D_ID.data(), &error);
    cl_mem ob_3 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, C_W_ID.data(), &error);
    cl_mem ob_4 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), &error);
    cl_mem ob_5 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), &error);
    cl_mem ob_6 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, C_DISCOUNT.data(), &error);
    cl_mem ob_7 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, C_CREDIT_LIM.data(), &error);
    cl_mem ob_8 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, C_BALANCE.data(), &error);
    cl_mem ob_9 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), &error);
    cl_mem ob_10 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, C_LAST.data(), &error);
    cl_mem ob_11 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, C_FIRST.data(), &error);
    cl_mem ob_12 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * queryCount, C_CREDIT.data(), &error);
    cl_mem ob_13 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_1.data(), &error);
    cl_mem ob_14 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_2.data(), &error);
    cl_mem ob_15 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, C_CITY.data(), &error);
    cl_mem ob_16 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * queryCount, C_STATE.data(), &error);
    cl_mem ob_17 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray9) * queryCount, C_ZIP.data(), &error);
    cl_mem ob_18 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * queryCount, C_PHONE.data(), &error);
    cl_mem ob_19 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray10) * queryCount, C_SINCE.data(), &error);
    cl_mem ob_20 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * queryCount, C_MIDDLE.data(), &error);
    cl_mem ob_21 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray30) * queryCount, C_DATA.data(), &error);

    
    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &ib_1);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &ob_1);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &ib_2);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &ob_2);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &ib_3);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &ob_3);

    clSetKernelArg(kernel[3], 0, sizeof(cl_mem), &ib_4);
    clSetKernelArg(kernel[3], 1, sizeof(cl_mem), &ob_4);

    clSetKernelArg(kernel[4], 0, sizeof(cl_mem), &ib_5);
    clSetKernelArg(kernel[4], 1, sizeof(cl_mem), &ob_5);

    clSetKernelArg(kernel[5], 0, sizeof(cl_mem), &ib_6);
    clSetKernelArg(kernel[5], 1, sizeof(cl_mem), &ob_6);

    clSetKernelArg(kernel[6], 0, sizeof(cl_mem), &ib_7);
    clSetKernelArg(kernel[6], 1, sizeof(cl_mem), &ob_7);

    clSetKernelArg(kernel[7], 0, sizeof(cl_mem), &ib_8);
    clSetKernelArg(kernel[7], 1, sizeof(cl_mem), &ob_8);

    clSetKernelArg(kernel[8], 0, sizeof(cl_mem), &ib_9);
    clSetKernelArg(kernel[8], 1, sizeof(cl_mem), &ob_9);

    clSetKernelArg(kernel[9], 0, sizeof(cl_mem), &ib_10);
    clSetKernelArg(kernel[9], 1, sizeof(cl_mem), &ob_10);

    clSetKernelArg(kernel[10], 0, sizeof(cl_mem), &ib_11);
    clSetKernelArg(kernel[10], 1, sizeof(cl_mem), &ob_11);

    clSetKernelArg(kernel[11], 0, sizeof(cl_mem), &ib_12);
    clSetKernelArg(kernel[11], 1, sizeof(cl_mem), &ob_12);

    clSetKernelArg(kernel[12], 0, sizeof(cl_mem), &ib_13);
    clSetKernelArg(kernel[12], 1, sizeof(cl_mem), &ob_13);

    clSetKernelArg(kernel[13], 0, sizeof(cl_mem), &ib_14);
    clSetKernelArg(kernel[13], 1, sizeof(cl_mem), &ob_14);

    clSetKernelArg(kernel[14], 0, sizeof(cl_mem), &ib_15);
    clSetKernelArg(kernel[14], 1, sizeof(cl_mem), &ob_15);

    clSetKernelArg(kernel[15], 0, sizeof(cl_mem), &ib_16);
    clSetKernelArg(kernel[15], 1, sizeof(cl_mem), &ob_16);

    clSetKernelArg(kernel[16], 0, sizeof(cl_mem), &ib_17);
    clSetKernelArg(kernel[16], 1, sizeof(cl_mem), &ob_17);

    clSetKernelArg(kernel[17], 0, sizeof(cl_mem), &ib_18);
    clSetKernelArg(kernel[17], 1, sizeof(cl_mem), &ob_18);

    clSetKernelArg(kernel[18], 0, sizeof(cl_mem), &ib_19);
    clSetKernelArg(kernel[18], 1, sizeof(cl_mem), &ob_19);

    clSetKernelArg(kernel[19], 0, sizeof(cl_mem), &ib_20);
    clSetKernelArg(kernel[19], 1, sizeof(cl_mem), &ob_20);

    clSetKernelArg(kernel[20], 0, sizeof(cl_mem), &ib_21);
    clSetKernelArg(kernel[20], 1, sizeof(cl_mem), &ob_21);
    
    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    unsigned long start = 0;
    unsigned long end = 0;

    cl_event transfer;
    //clEnqueueWriteBuffer (queueGPU, ib_1, CL_TRUE, 0, sizeof(int) * queryCount, inputData.C_ID.data(), 0, nullptr, &transfer);
    clEnqueueWriteBuffer (queueGPU, ib_6, CL_TRUE, 0, sizeof(float) * queryCount, inputData.C_DISCOUNT.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ib_17, CL_TRUE, 0, sizeof(charArray9) * queryCount, inputData.C_ZIP.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ib_18, CL_TRUE, 0, sizeof(charArray20) * queryCount, inputData.C_PHONE.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ib_19, CL_TRUE, 0, sizeof(charArray10) * queryCount, inputData.C_SINCE.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_20, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_SINCE.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * queryCount, C_DATA.data(), 0, nullptr, &transfer);
    clWaitForEvents(1, &transfer);

    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);

    unsigned long transferTime = end - start;

    size_t globalWorkSize[] = {queryCount};

    unsigned long kernelTime = 0;
    
    cl_event event[21];
    for (int i = 0; i < 21; i++) {
        CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        //unsigned long start = 0;
        //unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);       
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }

    clFinish(queueGPU);

    CheckError(clEnqueueReadBuffer(queueGPU, ob_1, CL_TRUE, 0, sizeof(int)*queryCount, C_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_2, CL_TRUE, 0, sizeof(int)*queryCount, C_D_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_3, CL_TRUE, 0, sizeof(int)*queryCount, C_W_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_4, CL_TRUE, 0, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_5, CL_TRUE, 0, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_6, CL_TRUE, 0, sizeof(float)*queryCount, C_DISCOUNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_7, CL_TRUE, 0, sizeof(float)*queryCount, C_CREDIT_LIM.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_8, CL_TRUE, 0, sizeof(float)*queryCount, C_BALANCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_9, CL_TRUE, 0, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_LAST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_11, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_FIRST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_CREDIT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_13, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_1.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_14, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_2.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_15, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_CITY.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_STATE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * queryCount, C_ZIP.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_18, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_PHONE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * queryCount, C_SINCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_20, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_MIDDLE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * queryCount, C_DATA.data(), 0, nullptr, nullptr));
    
    clReleaseCommandQueue(queueGPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseMemObject(ib_1);
    clReleaseMemObject(ib_2);
    clReleaseMemObject(ib_3);
    clReleaseMemObject(ib_4);
    clReleaseMemObject(ib_5);
    clReleaseMemObject(ib_6);
    clReleaseMemObject(ib_7);
    clReleaseMemObject(ib_8);
    clReleaseMemObject(ib_9);
    clReleaseMemObject(ib_10);
    clReleaseMemObject(ib_11);
    clReleaseMemObject(ib_12);
    clReleaseMemObject(ib_13);
    clReleaseMemObject(ib_14);
    clReleaseMemObject(ib_15);
    clReleaseMemObject(ib_16);
    clReleaseMemObject(ib_17);
    clReleaseMemObject(ib_18);
    clReleaseMemObject(ib_19);
    clReleaseMemObject(ib_20);
    clReleaseMemObject(ib_21);

    clReleaseMemObject(ob_1);
    clReleaseMemObject(ob_2);
    clReleaseMemObject(ob_3);
    clReleaseMemObject(ob_4);
    clReleaseMemObject(ob_5);
    clReleaseMemObject(ob_6);
    clReleaseMemObject(ob_7);
    clReleaseMemObject(ob_8);
    clReleaseMemObject(ob_9);
    clReleaseMemObject(ob_10);
    clReleaseMemObject(ob_11);
    clReleaseMemObject(ob_12);
    clReleaseMemObject(ob_13);
    clReleaseMemObject(ob_14);
    clReleaseMemObject(ob_15);
    clReleaseMemObject(ob_16);
    clReleaseMemObject(ob_17);
    clReleaseMemObject(ob_18);
    clReleaseMemObject(ob_19);
    clReleaseMemObject(ob_20);
    clReleaseMemObject(ob_21);

    clReleaseProgram(GPUprogram_int_insert);
    clReleaseProgram(GPUprogram_char2_insert);
    clReleaseProgram(GPUprogram_char9_insert);
    clReleaseProgram(GPUprogram_char10_insert);
    clReleaseProgram(GPUprogram_char20_insert);
    clReleaseProgram(GPUprogram_char30_insert);
    clReleaseProgram(GPUprogram_float_insert);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    if (!includeTransferTime) return transferTime/1000000.0;
    else return timeInSeconds/1000;
}

double OpenCLExecutionColumnStore::InsertColumnStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, InputDataInsertColumnStore inputData, bool includeTransferTime) {
    
    //std::cout << "Executing insert for column store on CPU" << std::endl;
    
    std::vector<int> C_ID(queryCount);
    std::vector<int> C_D_ID(queryCount);
    std::vector<int> C_W_ID(queryCount);
    std::vector<float> C_DISCOUNT(queryCount);
    std::vector<float> C_CREDIT_LIM(queryCount);
    std::vector<charArray20> C_LAST(queryCount);
    std::vector<charArray20> C_FIRST(queryCount);
    std::vector<charArray2> C_CREDIT(queryCount);
    std::vector<float> C_BALANCE(queryCount);
    std::vector<float> C_YTD_PAYMENT(queryCount);
    std::vector<int> C_PAYMENT_CNT(queryCount);
    std::vector<int> C_DELIVERY_CNT(queryCount);
    std::vector<charArray20> C_STREET_1(queryCount);
    std::vector<charArray20> C_STREET_2(queryCount);
    std::vector<charArray20> C_CITY(queryCount);
    std::vector<charArray2> C_STATE(queryCount);
    std::vector<charArray9> C_ZIP(queryCount);
    std::vector<charArray20> C_PHONE(queryCount);
    std::vector<charArray10> C_SINCE(queryCount);
    std::vector<charArray2> C_MIDDLE(queryCount);
    std::vector<charArray30> C_DATA(queryCount);

    cl_int error = CL_SUCCESS;

    cl_program CPUprogram_char2_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char2_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char9_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char9_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char10_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char10_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char20_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char20_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char30_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char30_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_int_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_int_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_float_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_float_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    //auto startTime = std::chrono::high_resolution_clock::now();

    cl_kernel kernel[21];

    for (int i = 0; i < 5; i++) {
        kernel[i] = clCreateKernel(CPUprogram_int_insert, "op_int_insert", &error);
    }
    for (int i = 5; i < 9; i++) {
        kernel[i] = clCreateKernel(CPUprogram_float_insert, "op_float_insert", &error);
    }
    kernel[9] = clCreateKernel(CPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[10] = clCreateKernel(CPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[11] = clCreateKernel(CPUprogram_char2_insert, "op_char2_insert", &error);
    kernel[12] = clCreateKernel(CPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[13] = clCreateKernel(CPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[14] = clCreateKernel(CPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[15] = clCreateKernel(CPUprogram_char2_insert, "op_char2_insert", &error);
    kernel[16] = clCreateKernel(CPUprogram_char9_insert, "op_char9_insert", &error);
    kernel[17] = clCreateKernel(CPUprogram_char20_insert, "op_char20_insert", &error);
    kernel[18] = clCreateKernel(CPUprogram_char10_insert, "op_char10_insert", &error);
    kernel[19] = clCreateKernel(CPUprogram_char2_insert, "op_char2_insert", &error);
    kernel[20] = clCreateKernel(CPUprogram_char30_insert, "op_char30_insert", &error);
    
    cl_mem ib_1 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData.C_ID.data(), &error);
    cl_mem ib_2 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData.C_D_ID.data(), &error);
    cl_mem ib_3 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData.C_W_ID.data(), &error);
    cl_mem ib_4 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData.C_PAYMENT_CNT.data(), &error);
    cl_mem ib_5 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData.C_DELIVERY_CNT.data(), &error);
    cl_mem ib_6 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, inputData.C_DISCOUNT.data(), &error);
    cl_mem ib_7 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, inputData.C_CREDIT_LIM.data(), &error);
    cl_mem ib_8 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, inputData.C_BALANCE.data(), &error);
    cl_mem ib_9 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, inputData.C_YTD_PAYMENT.data(), &error);
    cl_mem ib_10 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_LAST.data(), &error);
    cl_mem ib_11 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_FIRST.data(), &error);
    cl_mem ib_12 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, inputData.C_CREDIT.data(), &error);
    cl_mem ib_13 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_STREET_1.data(), &error);
    cl_mem ib_14 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_STREET_2.data(), &error);
    cl_mem ib_15 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_CITY.data(), &error);
    cl_mem ib_16 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, inputData.C_STATE.data(), &error);
    cl_mem ib_17 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray9) * queryCount, inputData.C_ZIP.data(), &error);
    cl_mem ib_18 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, inputData.C_PHONE.data(), &error);
    cl_mem ib_19 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray10) * queryCount, inputData.C_SINCE.data(), &error);
    cl_mem ib_20 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, inputData.C_MIDDLE.data(), &error);
    cl_mem ib_21 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray30) * queryCount, inputData.C_DATA.data(), &error);

    cl_mem ob_1 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_ID.data(), &error);
    cl_mem ob_2 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_D_ID.data(), &error);
    cl_mem ob_3 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_W_ID.data(), &error);
    cl_mem ob_4 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), &error);
    cl_mem ob_5 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), &error);
    cl_mem ob_6 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_DISCOUNT.data(), &error);
    cl_mem ob_7 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_CREDIT_LIM.data(), &error);
    cl_mem ob_8 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_BALANCE.data(), &error);
    cl_mem ob_9 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), &error);
    cl_mem ob_10 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_LAST.data(), &error);
    cl_mem ob_11 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_FIRST.data(), &error);
    cl_mem ob_12 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_CREDIT.data(), &error);
    cl_mem ob_13 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_1.data(), &error);
    cl_mem ob_14 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_2.data(), &error);
    cl_mem ob_15 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_CITY.data(), &error);
    cl_mem ob_16 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_STATE.data(), &error);
    cl_mem ob_17 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray9) * queryCount, C_ZIP.data(), &error);
    cl_mem ob_18 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_PHONE.data(), &error);
    cl_mem ob_19 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray10) * queryCount, C_SINCE.data(), &error);
    cl_mem ob_20 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_MIDDLE.data(), &error);
    cl_mem ob_21 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray30) * queryCount, C_DATA.data(), &error);

    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &ib_1);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &ob_1);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &ib_2);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &ob_2);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &ib_3);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &ob_3);

    clSetKernelArg(kernel[3], 0, sizeof(cl_mem), &ib_4);
    clSetKernelArg(kernel[3], 1, sizeof(cl_mem), &ob_4);

    clSetKernelArg(kernel[4], 0, sizeof(cl_mem), &ib_5);
    clSetKernelArg(kernel[4], 1, sizeof(cl_mem), &ob_5);

    clSetKernelArg(kernel[5], 0, sizeof(cl_mem), &ib_6);
    clSetKernelArg(kernel[5], 1, sizeof(cl_mem), &ob_6);

    clSetKernelArg(kernel[6], 0, sizeof(cl_mem), &ib_7);
    clSetKernelArg(kernel[6], 1, sizeof(cl_mem), &ob_7);

    clSetKernelArg(kernel[7], 0, sizeof(cl_mem), &ib_8);
    clSetKernelArg(kernel[7], 1, sizeof(cl_mem), &ob_8);

    clSetKernelArg(kernel[8], 0, sizeof(cl_mem), &ib_9);
    clSetKernelArg(kernel[8], 1, sizeof(cl_mem), &ob_9);

    clSetKernelArg(kernel[9], 0, sizeof(cl_mem), &ib_10);
    clSetKernelArg(kernel[9], 1, sizeof(cl_mem), &ob_10);

    clSetKernelArg(kernel[10], 0, sizeof(cl_mem), &ib_11);
    clSetKernelArg(kernel[10], 1, sizeof(cl_mem), &ob_11);

    clSetKernelArg(kernel[11], 0, sizeof(cl_mem), &ib_12);
    clSetKernelArg(kernel[11], 1, sizeof(cl_mem), &ob_12);

    clSetKernelArg(kernel[12], 0, sizeof(cl_mem), &ib_13);
    clSetKernelArg(kernel[12], 1, sizeof(cl_mem), &ob_13);

    clSetKernelArg(kernel[13], 0, sizeof(cl_mem), &ib_14);
    clSetKernelArg(kernel[13], 1, sizeof(cl_mem), &ob_14);

    clSetKernelArg(kernel[14], 0, sizeof(cl_mem), &ib_15);
    clSetKernelArg(kernel[14], 1, sizeof(cl_mem), &ob_15);

    clSetKernelArg(kernel[15], 0, sizeof(cl_mem), &ib_16);
    clSetKernelArg(kernel[15], 1, sizeof(cl_mem), &ob_16);

    clSetKernelArg(kernel[16], 0, sizeof(cl_mem), &ib_17);
    clSetKernelArg(kernel[16], 1, sizeof(cl_mem), &ob_17);

    clSetKernelArg(kernel[17], 0, sizeof(cl_mem), &ib_18);
    clSetKernelArg(kernel[17], 1, sizeof(cl_mem), &ob_18);

    clSetKernelArg(kernel[18], 0, sizeof(cl_mem), &ib_19);
    clSetKernelArg(kernel[18], 1, sizeof(cl_mem), &ob_19);

    clSetKernelArg(kernel[19], 0, sizeof(cl_mem), &ib_20);
    clSetKernelArg(kernel[19], 1, sizeof(cl_mem), &ob_20);

    clSetKernelArg(kernel[20], 0, sizeof(cl_mem), &ib_21);
    clSetKernelArg(kernel[20], 1, sizeof(cl_mem), &ob_21);
    
    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {queryCount};
    unsigned long kernelTime = 0;
    
    cl_event event[21];
    for (int i = 0; i < 21; i++) {
        CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);       
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }

    clFinish(queueCPU);

   /* CheckError(clEnqueueReadBuffer(queueCPU, ob_1, CL_TRUE, 0, sizeof(int)*queryCount, C_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_2, CL_TRUE, 0, sizeof(int)*queryCount, C_D_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_3, CL_TRUE, 0, sizeof(int)*queryCount, C_W_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_4, CL_TRUE, 0, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_5, CL_TRUE, 0, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_6, CL_TRUE, 0, sizeof(float)*queryCount, C_DISCOUNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_7, CL_TRUE, 0, sizeof(float)*queryCount, C_CREDIT_LIM.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_8, CL_TRUE, 0, sizeof(float)*queryCount, C_BALANCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_9, CL_TRUE, 0, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_LAST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_11, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_FIRST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_CREDIT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_13, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_1.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_14, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_2.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_15, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_CITY.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_STATE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * queryCount, C_ZIP.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_18, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_PHONE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * queryCount, C_SINCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_20, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_MIDDLE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * queryCount, C_DATA.data(), 0, nullptr, nullptr));*/


    clReleaseCommandQueue(queueCPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseMemObject(ib_1);
    clReleaseMemObject(ib_2);
    clReleaseMemObject(ib_3);
    clReleaseMemObject(ib_4);
    clReleaseMemObject(ib_5);
    clReleaseMemObject(ib_6);
    clReleaseMemObject(ib_7);
    clReleaseMemObject(ib_8);
    clReleaseMemObject(ib_9);
    clReleaseMemObject(ib_10);
    clReleaseMemObject(ib_11);
    clReleaseMemObject(ib_12);
    clReleaseMemObject(ib_13);
    clReleaseMemObject(ib_14);
    clReleaseMemObject(ib_15);
    clReleaseMemObject(ib_16);
    clReleaseMemObject(ib_17);
    clReleaseMemObject(ib_18);
    clReleaseMemObject(ib_19);
    clReleaseMemObject(ib_20);
    clReleaseMemObject(ib_21);

    clReleaseMemObject(ob_1);
    clReleaseMemObject(ob_2);
    clReleaseMemObject(ob_3);
    clReleaseMemObject(ob_4);
    clReleaseMemObject(ob_5);
    clReleaseMemObject(ob_6);
    clReleaseMemObject(ob_7);
    clReleaseMemObject(ob_8);
    clReleaseMemObject(ob_9);
    clReleaseMemObject(ob_10);
    clReleaseMemObject(ob_11);
    clReleaseMemObject(ob_12);
    clReleaseMemObject(ob_13);
    clReleaseMemObject(ob_14);
    clReleaseMemObject(ob_15);
    clReleaseMemObject(ob_16);
    clReleaseMemObject(ob_17);
    clReleaseMemObject(ob_18);
    clReleaseMemObject(ob_19);
    clReleaseMemObject(ob_20);
    clReleaseMemObject(ob_21);


    clReleaseProgram(CPUprogram_int_insert);
    clReleaseProgram(CPUprogram_char2_insert);
    clReleaseProgram(CPUprogram_char9_insert);
    clReleaseProgram(CPUprogram_char10_insert);
    clReleaseProgram(CPUprogram_char20_insert);
    clReleaseProgram(CPUprogram_char30_insert);
    clReleaseProgram(CPUprogram_float_insert);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    //cout << "time taken" << double(timeInSeconds) << endl;

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}


double OpenCLExecutionColumnStore::UpdateColumnStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime) {
    
    //std::cout << "Executing update for column store on GPU" << std::endl;
    
    cl_int error = CL_SUCCESS;

    cl_program GPUprogram_int_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_update.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_int_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_float_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_update.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_float_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_char2_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_update.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char2_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char9_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_update.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char9_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char10_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_update.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char10_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char20_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_update.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char20_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char30_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_update.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char30_update, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    float timeInSeconds;
    //auto startTime = std::chrono::high_resolution_clock::now();

    cl_kernel kernel[21];

    for (int i = 0; i < 5; i++) {
        kernel[i] = clCreateKernel(GPUprogram_int_update, "op_int_update", &error);
    }
    for (int i = 5; i < 9; i++) {
        kernel[i] = clCreateKernel(GPUprogram_float_update, "op_float_update", &error);
    }
    kernel[9] = clCreateKernel(GPUprogram_char20_update, "op_char20_update", &error);
    kernel[10] = clCreateKernel(GPUprogram_char20_update, "op_char20_update", &error);
    kernel[11] = clCreateKernel(GPUprogram_char2_update, "op_char2_update", &error);
    kernel[12] = clCreateKernel(GPUprogram_char20_update, "op_char20_update", &error);
    kernel[13] = clCreateKernel(GPUprogram_char20_update, "op_char20_update", &error);
    kernel[14] = clCreateKernel(GPUprogram_char20_update, "op_char20_update", &error);
    kernel[15] = clCreateKernel(GPUprogram_char2_update, "op_char2_update", &error);
    kernel[16] = clCreateKernel(GPUprogram_char9_update, "op_char9_update", &error);
    kernel[17] = clCreateKernel(GPUprogram_char20_update, "op_char20_update", &error);
    kernel[18] = clCreateKernel(GPUprogram_char10_update, "op_char10_update", &error);
    kernel[19] = clCreateKernel(GPUprogram_char2_update, "op_char2_update", &error);
    kernel[20] = clCreateKernel(GPUprogram_char30_update, "op_char30_update", &error);

    int cust_size = loadedData->C_ID.size();

    cl_mem ib_c = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*queryCount, inputData, &error);

    cl_mem ob_1 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*cust_size, loadedData->C_ID.data(), &error);
    cl_mem ob_2 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*cust_size, loadedData->C_D_ID.data(), &error);
    cl_mem ob_3 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*cust_size, loadedData->C_W_ID.data(), &error);
    cl_mem ob_4 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*cust_size, loadedData->C_PAYMENT_CNT.data(), &error);
    cl_mem ob_5 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*cust_size, loadedData->C_DELIVERY_CNT.data(), &error);
    cl_mem ob_6 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*cust_size, loadedData->C_DISCOUNT.data(), &error);
    cl_mem ob_7 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*cust_size, loadedData->C_CREDIT_LIM.data(), &error);
    cl_mem ob_8 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*cust_size, loadedData->C_BALANCE.data(), &error);
    cl_mem ob_9 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float)*cust_size, loadedData->C_YTD_PAYMENT.data(), &error);
    cl_mem ob_10 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), &error);
    cl_mem ob_11 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_FIRST.data(), &error);
    cl_mem ob_12 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), &error);
    cl_mem ob_13 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_1.data(), &error);
    cl_mem ob_14 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_2.data(), &error);
    cl_mem ob_15 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_CITY.data(), &error);
    cl_mem ob_16 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_STATE.data(), &error);
    cl_mem ob_17 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), &error);
    cl_mem ob_18 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_PHONE.data(), &error);
    cl_mem ob_19 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), &error);
    cl_mem ob_20 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_MIDDLE.data(), &error);
    cl_mem ob_21 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), &error);

    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &ob_1);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &ob_2);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &ob_3);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[3], 0, sizeof(cl_mem), &ob_4);
    clSetKernelArg(kernel[3], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[4], 0, sizeof(cl_mem), &ob_5);
    clSetKernelArg(kernel[4], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[5], 0, sizeof(cl_mem), &ob_6);
    clSetKernelArg(kernel[5], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[6], 0, sizeof(cl_mem), &ob_7);
    clSetKernelArg(kernel[6], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[7], 0, sizeof(cl_mem), &ob_8);
    clSetKernelArg(kernel[7], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[8], 0, sizeof(cl_mem), &ob_9);
    clSetKernelArg(kernel[8], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[9], 0, sizeof(cl_mem), &ob_10);
    clSetKernelArg(kernel[9], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[10], 0, sizeof(cl_mem), &ob_11);
    clSetKernelArg(kernel[10], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[11], 0, sizeof(cl_mem), &ob_12);
    clSetKernelArg(kernel[11], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[12], 0, sizeof(cl_mem), &ob_13);
    clSetKernelArg(kernel[12], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[13], 0, sizeof(cl_mem), &ob_14);
    clSetKernelArg(kernel[13], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[14], 0, sizeof(cl_mem), &ob_15);
    clSetKernelArg(kernel[14], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[15], 0, sizeof(cl_mem), &ob_16);
    clSetKernelArg(kernel[15], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[16], 0, sizeof(cl_mem), &ob_17);
    clSetKernelArg(kernel[16], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[17], 0, sizeof(cl_mem), &ob_18);
    clSetKernelArg(kernel[17], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[18], 0, sizeof(cl_mem), &ob_19);
    clSetKernelArg(kernel[18], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[19], 0, sizeof(cl_mem), &ob_20);
    clSetKernelArg(kernel[19], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[20], 0, sizeof(cl_mem), &ob_21);
    clSetKernelArg(kernel[20], 1, sizeof(cl_mem), &ib_c);

    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    /*cl_event transfer;
    //clEnqueueWriteBuffer (queueGPU, ib_c, CL_TRUE, 0, sizeof(int) * queryCount, inputData, 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_1, CL_TRUE, 0, sizeof(int) * cust_size, loadedData->C_ID.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_6, CL_TRUE, 0, sizeof(float) * cust_size, loadedData->C_DISCOUNT.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), 0, nullptr, &transfer);
    clEnqueueWriteBuffer (queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), 0, nullptr, &transfer);

    clWaitForEvents(1, &transfer);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;*/

    size_t globalWorkSize[] = {queryCount};

    unsigned long kernelTime = 0;

    cl_event event[21];
    for (int i = 0; i < 21; i++) {
    	CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
    	clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);       
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }
    
    /*CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[0], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[1], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[2], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[3], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[4], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[5], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[6], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[7], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[8], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[9], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[10], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[11], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[12], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[13], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[14], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[15], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[16], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[17], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[18], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[19], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[20], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));*/

    clFinish(queueGPU);
    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    
    //cout << cust_size << endl;

    cl_event transfer;
    //clEnqueueReadBuffer(queueGPU, ob_1, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_ID.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_6, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_DISCOUNT.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_STATE.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), 0, nullptr, &transfer);
    clEnqueueReadBuffer(queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), 0, nullptr, &transfer);

    clWaitForEvents(1, &transfer);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;

    /*CheckError(clEnqueueReadBuffer(queueGPU, ob_1, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_2, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_D_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_3, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_W_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_4, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_PAYMENT_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_5, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_DELIVERY_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_6, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_DISCOUNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_7, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_CREDIT_LIM.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_8, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_BALANCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_9, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_YTD_PAYMENT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_11, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_FIRST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_13, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_STREET_1.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_14, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_STREET_2.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_15, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_CITY.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_STATE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_18, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_PHONE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_20, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_MIDDLE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), 0, nullptr, nullptr));*/

    clReleaseCommandQueue(queueGPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseProgram(GPUprogram_float_update);
    clReleaseProgram(GPUprogram_int_update);
    clReleaseProgram(GPUprogram_char2_update);
    clReleaseProgram(GPUprogram_char9_update);
    clReleaseProgram(GPUprogram_char10_update);
    clReleaseProgram(GPUprogram_char20_update);
    clReleaseProgram(GPUprogram_char30_update);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }
    clReleaseMemObject(ib_c);

    clReleaseMemObject(ob_1);
    clReleaseMemObject(ob_2);
    clReleaseMemObject(ob_3);
    clReleaseMemObject(ob_4);
    clReleaseMemObject(ob_5);
    clReleaseMemObject(ob_6);
    clReleaseMemObject(ob_7);
    clReleaseMemObject(ob_8);
    clReleaseMemObject(ob_9);
    clReleaseMemObject(ob_10);
    clReleaseMemObject(ob_11);
    clReleaseMemObject(ob_12);
    clReleaseMemObject(ob_13);
    clReleaseMemObject(ob_14);
    clReleaseMemObject(ob_15);
    clReleaseMemObject(ob_16);
    clReleaseMemObject(ob_17);
    clReleaseMemObject(ob_18);
    clReleaseMemObject(ob_19);
    clReleaseMemObject(ob_20);
    clReleaseMemObject(ob_21);
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    //cout << timeInSeconds/1000 << endl;
    
    if (!includeTransferTime) return transferTime/1000000.0;
    else return timeInSeconds/1000;
}

double OpenCLExecutionColumnStore::UpdateColumnStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime) {

    //std::cout << "Executing update for column store on CPU" << std::endl;
    
    cl_int error = CL_SUCCESS;

    cl_program CPUprogram_int_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_update.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_int_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_float_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_update.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_float_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_char2_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_update.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char2_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char9_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_update.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char9_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char10_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_update.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char10_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char20_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_update.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char20_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char30_update = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_update.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char30_update, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    //auto startTime = std::chrono::high_resolution_clock::now();


    cl_kernel kernel[21];

    for (int i = 0; i < 5; i++) {
        kernel[i] = clCreateKernel(CPUprogram_int_update, "op_int_update", &error);
    }
    for (int i = 5; i < 9; i++) {
        kernel[i] = clCreateKernel(CPUprogram_float_update, "op_float_update", &error);
    }
    kernel[9] = clCreateKernel(CPUprogram_char20_update, "op_char20_update", &error);
    kernel[10] = clCreateKernel(CPUprogram_char20_update, "op_char20_update", &error);
    kernel[11] = clCreateKernel(CPUprogram_char2_update, "op_char2_update", &error);
    kernel[12] = clCreateKernel(CPUprogram_char20_update, "op_char20_update", &error);
    kernel[13] = clCreateKernel(CPUprogram_char20_update, "op_char20_update", &error);
    kernel[14] = clCreateKernel(CPUprogram_char20_update, "op_char20_update", &error);
    kernel[15] = clCreateKernel(CPUprogram_char2_update, "op_char2_update", &error);
    kernel[16] = clCreateKernel(CPUprogram_char9_update, "op_char9_update", &error);
    kernel[17] = clCreateKernel(CPUprogram_char20_update, "op_char20_update", &error);
    kernel[18] = clCreateKernel(CPUprogram_char10_update, "op_char10_update", &error);
    kernel[19] = clCreateKernel(CPUprogram_char2_update, "op_char2_update", &error);
    kernel[20] = clCreateKernel(CPUprogram_char30_update, "op_char30_update", &error);

    int cust_size = loadedData->C_D_ID.size();

    cl_mem ib_c = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData, &error);

    cl_mem ob_1 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_ID.data(), &error);
    cl_mem ob_2 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_D_ID.data(), &error);
    cl_mem ob_3 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_W_ID.data(), &error);
    cl_mem ob_4 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_PAYMENT_CNT.data(), &error);
    cl_mem ob_5 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_DELIVERY_CNT.data(), &error);
    cl_mem ob_6 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_DISCOUNT.data(), &error);
    cl_mem ob_7 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_CREDIT_LIM.data(), &error);
    cl_mem ob_8 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_BALANCE.data(), &error);
    cl_mem ob_9 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_YTD_PAYMENT.data(), &error);
    cl_mem ob_10 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), &error);
    cl_mem ob_11 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_FIRST.data(), &error);
    cl_mem ob_12 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), &error);
    cl_mem ob_13 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_1.data(), &error);
    cl_mem ob_14 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_2.data(), &error);
    cl_mem ob_15 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_CITY.data(), &error);
    cl_mem ob_16 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_STATE.data(), &error);
    cl_mem ob_17 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), &error);
    cl_mem ob_18 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_PHONE.data(), &error);
    cl_mem ob_19 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), &error);
    cl_mem ob_20 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_MIDDLE.data(), &error);
    cl_mem ob_21 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), &error);

    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &ob_1);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &ob_2);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &ob_3);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[3], 0, sizeof(cl_mem), &ob_4);
    clSetKernelArg(kernel[3], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[4], 0, sizeof(cl_mem), &ob_5);
    clSetKernelArg(kernel[4], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[5], 0, sizeof(cl_mem), &ob_6);
    clSetKernelArg(kernel[5], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[6], 0, sizeof(cl_mem), &ob_7);
    clSetKernelArg(kernel[6], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[7], 0, sizeof(cl_mem), &ob_8);
    clSetKernelArg(kernel[7], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[8], 0, sizeof(cl_mem), &ob_9);
    clSetKernelArg(kernel[8], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[9], 0, sizeof(cl_mem), &ob_10);
    clSetKernelArg(kernel[9], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[10], 0, sizeof(cl_mem), &ob_11);
    clSetKernelArg(kernel[10], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[11], 0, sizeof(cl_mem), &ob_12);
    clSetKernelArg(kernel[11], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[12], 0, sizeof(cl_mem), &ob_13);
    clSetKernelArg(kernel[12], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[13], 0, sizeof(cl_mem), &ob_14);
    clSetKernelArg(kernel[13], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[14], 0, sizeof(cl_mem), &ob_15);
    clSetKernelArg(kernel[14], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[15], 0, sizeof(cl_mem), &ob_16);
    clSetKernelArg(kernel[15], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[16], 0, sizeof(cl_mem), &ob_17);
    clSetKernelArg(kernel[16], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[17], 0, sizeof(cl_mem), &ob_18);
    clSetKernelArg(kernel[17], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[18], 0, sizeof(cl_mem), &ob_19);
    clSetKernelArg(kernel[18], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[19], 0, sizeof(cl_mem), &ob_20);
    clSetKernelArg(kernel[19], 1, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[20], 0, sizeof(cl_mem), &ob_21);
    clSetKernelArg(kernel[20], 1, sizeof(cl_mem), &ib_c);
    
    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {queryCount};

    unsigned long kernelTime = 0;
    
    cl_event event[21];
    for (int i = 0; i < 21; i++) {
        CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);       
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }
    
    /*//CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[0], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[1], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
   // CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[2], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[3], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[4], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[5], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[6], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[7], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[8], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[9], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[10], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[11], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[12], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[13], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[14], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[15], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[16], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[17], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[18], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[19], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[20], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));*/

    clFinish(queueCPU);

    CheckError(clEnqueueReadBuffer(queueCPU, ob_1, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_2, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_D_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_3, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_W_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_4, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_PAYMENT_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_5, CL_TRUE, 0, sizeof(int)*cust_size, loadedData->C_DELIVERY_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_6, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_DISCOUNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_7, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_CREDIT_LIM.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_8, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_BALANCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_9, CL_TRUE, 0, sizeof(float)*cust_size, loadedData->C_YTD_PAYMENT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_11, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_FIRST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_13, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_STREET_1.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_14, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_STREET_2.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_15, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_CITY.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_STATE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_18, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_PHONE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_20, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_MIDDLE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), 0, nullptr, nullptr));

    clReleaseCommandQueue(queueCPU);
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseProgram(CPUprogram_float_update);
    clReleaseProgram(CPUprogram_int_update);
    clReleaseProgram(CPUprogram_char2_update);
    clReleaseProgram(CPUprogram_char9_update);
    clReleaseProgram(CPUprogram_char10_update);
    clReleaseProgram(CPUprogram_char20_update);
    clReleaseProgram(CPUprogram_char30_update);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }
    clReleaseMemObject(ib_c);

    clReleaseMemObject(ob_1);
    clReleaseMemObject(ob_2);
    clReleaseMemObject(ob_3);
    clReleaseMemObject(ob_4);
    clReleaseMemObject(ob_5);
    clReleaseMemObject(ob_6);
    clReleaseMemObject(ob_7);
    clReleaseMemObject(ob_8);
    clReleaseMemObject(ob_9);
    clReleaseMemObject(ob_10);
    clReleaseMemObject(ob_11);
    clReleaseMemObject(ob_12);
    clReleaseMemObject(ob_13);
    clReleaseMemObject(ob_14);
    clReleaseMemObject(ob_15);
    clReleaseMemObject(ob_16);
    clReleaseMemObject(ob_17);
    clReleaseMemObject(ob_18);
    clReleaseMemObject(ob_19);
    clReleaseMemObject(ob_20);
    clReleaseMemObject(ob_21);
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}

double OpenCLExecutionColumnStore::SelectColumnStoreGPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime) {

    //std::cout << "Executing select for column store on GPU" << std::endl;
    
    std::vector<int> C_ID(queryCount);
    std::vector<int> C_D_ID(queryCount);
    std::vector<int> C_W_ID(queryCount);
    std::vector<float> C_DISCOUNT(queryCount);
    std::vector<float> C_CREDIT_LIM(queryCount);
    std::vector<charArray20> C_LAST(queryCount);
    std::vector<charArray20> C_FIRST(queryCount);
    std::vector<charArray2> C_CREDIT(queryCount);
    std::vector<float> C_BALANCE(queryCount);
    std::vector<float> C_YTD_PAYMENT(queryCount);
    std::vector<int> C_PAYMENT_CNT(queryCount);
    std::vector<int> C_DELIVERY_CNT(queryCount);
    std::vector<charArray20> C_STREET_1(queryCount);
    std::vector<charArray20> C_STREET_2(queryCount);
    std::vector<charArray20> C_CITY(queryCount);
    std::vector<charArray2> C_STATE(queryCount);
    std::vector<charArray9> C_ZIP(queryCount);
    std::vector<charArray20> C_PHONE(queryCount);
    std::vector<charArray10> C_SINCE(queryCount);
    std::vector<charArray2> C_MIDDLE(queryCount);
    std::vector<charArray30> C_DATA(queryCount);

    cl_int error;

    cl_program GPUprogram_int_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_select.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_int_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_char2_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_select.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char2_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char9_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_select.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char9_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char10_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_select.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char10_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char20_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_select.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char20_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program GPUprogram_char30_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_select.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char30_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_float_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_select.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_float_select, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    float timeInSeconds;
    //auto startTime = std::chrono::high_resolution_clock::now();


    cl_kernel kernel[21];

    for (int i = 0; i < 5; i++) {
        kernel[i] = clCreateKernel(GPUprogram_int_select, "op_int_select", &error);
    }
    for (int i = 5; i < 9; i++) {
        kernel[i] = clCreateKernel(GPUprogram_float_select, "op_float_select", &error);
    }
    kernel[9] = clCreateKernel(GPUprogram_char20_select, "op_char20_select", &error);
    kernel[10] = clCreateKernel(GPUprogram_char20_select, "op_char20_select", &error);
    kernel[11] = clCreateKernel(GPUprogram_char2_select, "op_char2_select", &error);
    kernel[12] = clCreateKernel(GPUprogram_char20_select, "op_char20_select", &error);
    kernel[13] = clCreateKernel(GPUprogram_char20_select, "op_char20_select", &error);
    kernel[14] = clCreateKernel(GPUprogram_char20_select, "op_char20_select", &error);
    kernel[15] = clCreateKernel(GPUprogram_char2_select, "op_char2_select", &error);
    kernel[16] = clCreateKernel(GPUprogram_char9_select, "op_char9_select", &error);
    kernel[17] = clCreateKernel(GPUprogram_char20_select, "op_char20_select", &error);
    kernel[18] = clCreateKernel(GPUprogram_char10_select, "op_char10_select", &error);
    kernel[19] = clCreateKernel(GPUprogram_char2_select, "op_char2_select", &error);
    kernel[20] = clCreateKernel(GPUprogram_char30_select, "op_char30_select", &error);

    int cust_size = loadedData->C_ID.size();

    cl_mem ib_c = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData, &error);

    cl_mem ib_1 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_ID.data(), &error);
    cl_mem ib_2 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_D_ID.data(), &error);
    cl_mem ib_3 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_W_ID.data(), &error);
    cl_mem ib_4 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_PAYMENT_CNT.data(), &error);
    cl_mem ib_5 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_DELIVERY_CNT.data(), &error);
    cl_mem ib_6 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_DISCOUNT.data(), &error);
    cl_mem ib_7 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_CREDIT_LIM.data(), &error);
    cl_mem ib_8 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_BALANCE.data(), &error);
    cl_mem ib_9 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_YTD_PAYMENT.data(), &error);
    cl_mem ib_10 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), &error);
    cl_mem ib_11 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_FIRST.data(), &error);
    cl_mem ib_12 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), &error);
    cl_mem ib_13 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_1.data(), &error);
    cl_mem ib_14 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_2.data(), &error);
    cl_mem ib_15 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_CITY.data(), &error);
    cl_mem ib_16 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_STATE.data(), &error);
    cl_mem ib_17 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), &error);
    cl_mem ib_18 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_PHONE.data(), &error);
    cl_mem ib_19 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), &error);
    cl_mem ib_20 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_MIDDLE.data(), &error);
    cl_mem ib_21 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), &error);

    cl_mem ob_1 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_ID.data(), &error);
    cl_mem ob_2 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_D_ID.data(), &error);
    cl_mem ob_3 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_W_ID.data(), &error);
    cl_mem ob_4 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), &error);
    cl_mem ob_5 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), &error);
    cl_mem ob_6 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_DISCOUNT.data(), &error);
    cl_mem ob_7 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_CREDIT_LIM.data(), &error);
    cl_mem ob_8 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_BALANCE.data(), &error);
    cl_mem ob_9 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), &error);
    cl_mem ob_10 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_LAST.data(), &error);
    cl_mem ob_11 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_FIRST.data(), &error);
    cl_mem ob_12 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_CREDIT.data(), &error);
    cl_mem ob_13 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_1.data(), &error);
    cl_mem ob_14 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_2.data(), &error);
    cl_mem ob_15 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_CITY.data(), &error);
    cl_mem ob_16 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_STATE.data(), &error);
    cl_mem ob_17 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray9) * queryCount, C_ZIP.data(), &error);
    cl_mem ob_18 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_PHONE.data(), &error);
    cl_mem ob_19 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray10) * queryCount, C_SINCE.data(), &error);
    cl_mem ob_20 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_MIDDLE.data(), &error);
    cl_mem ob_21 = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray30) * queryCount, C_DATA.data(), &error);


    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &ib_1);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &ob_1);
    clSetKernelArg(kernel[0], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &ib_2);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &ob_2);
    clSetKernelArg(kernel[1], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &ib_3);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &ob_3);
    clSetKernelArg(kernel[2], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[3], 0, sizeof(cl_mem), &ib_4);
    clSetKernelArg(kernel[3], 1, sizeof(cl_mem), &ob_4);
    clSetKernelArg(kernel[3], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[4], 0, sizeof(cl_mem), &ib_5);
    clSetKernelArg(kernel[4], 1, sizeof(cl_mem), &ob_5);
    clSetKernelArg(kernel[4], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[5], 0, sizeof(cl_mem), &ib_6);
    clSetKernelArg(kernel[5], 1, sizeof(cl_mem), &ob_6);
    clSetKernelArg(kernel[5], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[6], 0, sizeof(cl_mem), &ib_7);
    clSetKernelArg(kernel[6], 1, sizeof(cl_mem), &ob_7);
    clSetKernelArg(kernel[6], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[7], 0, sizeof(cl_mem), &ib_8);
    clSetKernelArg(kernel[7], 1, sizeof(cl_mem), &ob_8);
    clSetKernelArg(kernel[7], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[8], 0, sizeof(cl_mem), &ib_9);
    clSetKernelArg(kernel[8], 1, sizeof(cl_mem), &ob_9);
    clSetKernelArg(kernel[8], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[9], 0, sizeof(cl_mem), &ib_10);
    clSetKernelArg(kernel[9], 1, sizeof(cl_mem), &ob_10);
    clSetKernelArg(kernel[9], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[10], 0, sizeof(cl_mem), &ib_11);
    clSetKernelArg(kernel[10], 1, sizeof(cl_mem), &ob_11);
    clSetKernelArg(kernel[10], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[11], 0, sizeof(cl_mem), &ib_12);
    clSetKernelArg(kernel[11], 1, sizeof(cl_mem), &ob_12);
    clSetKernelArg(kernel[11], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[12], 0, sizeof(cl_mem), &ib_13);
    clSetKernelArg(kernel[12], 1, sizeof(cl_mem), &ob_13);
    clSetKernelArg(kernel[12], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[13], 0, sizeof(cl_mem), &ib_14);
    clSetKernelArg(kernel[13], 1, sizeof(cl_mem), &ob_14);
    clSetKernelArg(kernel[13], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[14], 0, sizeof(cl_mem), &ib_15);
    clSetKernelArg(kernel[14], 1, sizeof(cl_mem), &ob_15);
    clSetKernelArg(kernel[14], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[15], 0, sizeof(cl_mem), &ib_16);
    clSetKernelArg(kernel[15], 1, sizeof(cl_mem), &ob_16);
    clSetKernelArg(kernel[15], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[16], 0, sizeof(cl_mem), &ib_17);
    clSetKernelArg(kernel[16], 1, sizeof(cl_mem), &ob_17);
    clSetKernelArg(kernel[16], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[17], 0, sizeof(cl_mem), &ib_18);
    clSetKernelArg(kernel[17], 1, sizeof(cl_mem), &ob_18);
    clSetKernelArg(kernel[17], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[18], 0, sizeof(cl_mem), &ib_19);
    clSetKernelArg(kernel[18], 1, sizeof(cl_mem), &ob_19);
    clSetKernelArg(kernel[18], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[19], 0, sizeof(cl_mem), &ib_20);
    clSetKernelArg(kernel[19], 1, sizeof(cl_mem), &ob_20);
    clSetKernelArg(kernel[19], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[20], 0, sizeof(cl_mem), &ib_21);
    clSetKernelArg(kernel[20], 1, sizeof(cl_mem), &ob_21);
    clSetKernelArg(kernel[20], 2, sizeof(cl_mem), &ib_c);

    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);


    /*cl_event transfer;
    //clEnqueueWriteBuffer (queueGPU, ib_c, CL_TRUE, 0, sizeof(int) * queryCount, inputData, 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_1, CL_TRUE, 0, sizeof(int) * cust_size, loadedData->C_ID.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_6, CL_TRUE, 0, sizeof(float) * cust_size, loadedData->C_DISCOUNT.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), 0, nullptr, &transfer);
    //clEnqueueWriteBuffer (queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), 0, nullptr, &transfer);

    clWaitForEvents(1, &transfer);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;*/

    size_t globalWorkSize[] = {queryCount};


    unsigned long kernelTime = 0;
    
    cl_event event[21];
    for (int i = 0; i < 21; i++) {
        CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);       
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }
    
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[0], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[1], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[2], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[3], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[4], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[5], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[6], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[7], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[8], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[9], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[10], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[11], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[12], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[13], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[14], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[15], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[16], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[17], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[18], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[19], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[20], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));

    clFinish(queueGPU);

    cl_event transfer;
    //clEnqueueReadBuffer(queueGPU, ob_1, CL_TRUE, 0, sizeof(int)*queryCount, C_ID.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_6, CL_TRUE, 0, sizeof(float)*queryCount, C_DISCOUNT.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_LAST.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_STATE.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * queryCount, C_ZIP.data(), 0, nullptr, &transfer);
    //clEnqueueReadBuffer(queueGPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * queryCount, C_SINCE.data(), 0, nullptr, &transfer);
    clEnqueueReadBuffer(queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * queryCount, C_DATA.data(), 0, nullptr, &transfer);

    clWaitForEvents(1, &transfer);
    unsigned long start = 0;
    unsigned long end = 0;
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_START, sizeof (cl_ulong), &start, NULL);
    clGetEventProfilingInfo(transfer, CL_PROFILING_COMMAND_END, sizeof (cl_ulong), &end, NULL);
    unsigned long transferTime = end - start;

    /*CheckError(clEnqueueReadBuffer(queueGPU, ob_1, CL_TRUE, 0, sizeof(int)*queryCount, C_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_2, CL_TRUE, 0, sizeof(int)*queryCount, C_D_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_3, CL_TRUE, 0, sizeof(int)*queryCount, C_W_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_4, CL_TRUE, 0, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_5, CL_TRUE, 0, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_6, CL_TRUE, 0, sizeof(float)*queryCount, C_DISCOUNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_7, CL_TRUE, 0, sizeof(float)*queryCount, C_CREDIT_LIM.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_8, CL_TRUE, 0, sizeof(float)*queryCount, C_BALANCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_9, CL_TRUE, 0, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_LAST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_11, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_FIRST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_CREDIT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_13, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_1.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_14, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_2.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_15, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_CITY.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_STATE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * queryCount, C_ZIP.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_18, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_PHONE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * queryCount, C_SINCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_20, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_MIDDLE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * queryCount, C_DATA.data(), 0, nullptr, nullptr));*/

    clReleaseCommandQueue(queueGPU);
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseProgram(GPUprogram_float_select);
    clReleaseProgram(GPUprogram_int_select);
    clReleaseProgram(GPUprogram_char2_select);
    clReleaseProgram(GPUprogram_char9_select);
    clReleaseProgram(GPUprogram_char10_select);
    clReleaseProgram(GPUprogram_char20_select);
    clReleaseProgram(GPUprogram_char30_select);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }

    clReleaseMemObject(ib_c);

    clReleaseMemObject(ib_1);
    clReleaseMemObject(ib_2);
    clReleaseMemObject(ib_3);
    clReleaseMemObject(ib_4);
    clReleaseMemObject(ib_5);
    clReleaseMemObject(ib_6);
    clReleaseMemObject(ib_7);
    clReleaseMemObject(ib_8);
    clReleaseMemObject(ib_9);
    clReleaseMemObject(ib_10);
    clReleaseMemObject(ib_11);
    clReleaseMemObject(ib_12);
    clReleaseMemObject(ib_13);
    clReleaseMemObject(ib_14);
    clReleaseMemObject(ib_15);
    clReleaseMemObject(ib_16);
    clReleaseMemObject(ib_17);
    clReleaseMemObject(ib_18);
    clReleaseMemObject(ib_19);
    clReleaseMemObject(ib_20);
    clReleaseMemObject(ib_21);

    clReleaseMemObject(ob_1);
    clReleaseMemObject(ob_2);
    clReleaseMemObject(ob_3);
    clReleaseMemObject(ob_4);
    clReleaseMemObject(ob_5);
    clReleaseMemObject(ob_6);
    clReleaseMemObject(ob_7);
    clReleaseMemObject(ob_8);
    clReleaseMemObject(ob_9);
    clReleaseMemObject(ob_10);
    clReleaseMemObject(ob_11);
    clReleaseMemObject(ob_12);
    clReleaseMemObject(ob_13);
    clReleaseMemObject(ob_14);
    clReleaseMemObject(ob_15);
    clReleaseMemObject(ob_16);
    clReleaseMemObject(ob_17);
    clReleaseMemObject(ob_18);
    clReleaseMemObject(ob_19);
    clReleaseMemObject(ob_20);
    clReleaseMemObject(ob_21);
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    if (!includeTransferTime) return transferTime/1000000.0;
    else return timeInSeconds/1000;
}

double OpenCLExecutionColumnStore::SelectColumnStoreCPU(cl_context CPUcontext, cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, cl_context GPUcontext, 
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, int* inputData, ColumnStoreData *loadedData, bool includeTransferTime) {

    //std::cout << "Executing select for column store on CPU" << std::endl;
    
    std::vector<int> C_ID(queryCount);
    std::vector<int> C_D_ID(queryCount);
    std::vector<int> C_W_ID(queryCount);
    std::vector<float> C_DISCOUNT(queryCount);
    std::vector<float> C_CREDIT_LIM(queryCount);
    std::vector<charArray20> C_LAST(queryCount);
    std::vector<charArray20> C_FIRST(queryCount);
    std::vector<charArray2> C_CREDIT(queryCount);
    std::vector<float> C_BALANCE(queryCount);
    std::vector<float> C_YTD_PAYMENT(queryCount);
    std::vector<int> C_PAYMENT_CNT(queryCount);
    std::vector<int> C_DELIVERY_CNT(queryCount);
    std::vector<charArray20> C_STREET_1(queryCount);
    std::vector<charArray20> C_STREET_2(queryCount);
    std::vector<charArray20> C_CITY(queryCount);
    std::vector<charArray2> C_STATE(queryCount);
    std::vector<charArray9> C_ZIP(queryCount);
    std::vector<charArray20> C_PHONE(queryCount);
    std::vector<charArray10> C_SINCE(queryCount);
    std::vector<charArray2> C_MIDDLE(queryCount);
    std::vector<charArray30> C_DATA(queryCount);

    cl_int error;

    cl_program CPUprogram_int_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_select.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_int_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_char2_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_select.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char2_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char9_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_select.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char9_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char10_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_select.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char10_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char20_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_select.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char20_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    cl_program CPUprogram_char30_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_select.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char30_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_float_select = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_select.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_float_select, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));
    
    float timeInSeconds;
    //auto startTime = std::chrono::high_resolution_clock::now();

    cl_kernel kernel[21];

    for (int i = 0; i < 5; i++) {
        kernel[i] = clCreateKernel(CPUprogram_int_select, "op_int_select", &error);
    }
    for (int i = 5; i < 9; i++) {
        kernel[i] = clCreateKernel(CPUprogram_float_select, "op_float_select", &error);
    }
    kernel[9] = clCreateKernel(CPUprogram_char20_select, "op_char20_select", &error);
    kernel[10] = clCreateKernel(CPUprogram_char20_select, "op_char20_select", &error);
    kernel[11] = clCreateKernel(CPUprogram_char2_select, "op_char2_select", &error);
    kernel[12] = clCreateKernel(CPUprogram_char20_select, "op_char20_select", &error);
    kernel[13] = clCreateKernel(CPUprogram_char20_select, "op_char20_select", &error);
    kernel[14] = clCreateKernel(CPUprogram_char20_select, "op_char20_select", &error);
    kernel[15] = clCreateKernel(CPUprogram_char2_select, "op_char2_select", &error);
    kernel[16] = clCreateKernel(CPUprogram_char9_select, "op_char9_select", &error);
    kernel[17] = clCreateKernel(CPUprogram_char20_select, "op_char20_select", &error);
    kernel[18] = clCreateKernel(CPUprogram_char10_select, "op_char10_select", &error);
    kernel[19] = clCreateKernel(CPUprogram_char2_select, "op_char2_select", &error);
    kernel[20] = clCreateKernel(CPUprogram_char30_select, "op_char30_select", &error);

    int cust_size = loadedData->C_ID.size();

    cl_mem ib_c = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, inputData, &error);

    cl_mem ib_1 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_ID.data(), &error);
    cl_mem ib_2 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_D_ID.data(), &error);
    cl_mem ib_3 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_W_ID.data(), &error);
    cl_mem ib_4 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_PAYMENT_CNT.data(), &error);
    cl_mem ib_5 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*cust_size, loadedData->C_DELIVERY_CNT.data(), &error);
    cl_mem ib_6 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_DISCOUNT.data(), &error);
    cl_mem ib_7 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_CREDIT_LIM.data(), &error);
    cl_mem ib_8 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_BALANCE.data(), &error);
    cl_mem ib_9 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*cust_size, loadedData->C_YTD_PAYMENT.data(), &error);
    cl_mem ib_10 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_LAST.data(), &error);
    cl_mem ib_11 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_FIRST.data(), &error);
    cl_mem ib_12 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_CREDIT.data(), &error);
    cl_mem ib_13 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_1.data(), &error);
    cl_mem ib_14 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_STREET_2.data(), &error);
    cl_mem ib_15 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_CITY.data(), &error);
    cl_mem ib_16 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_STATE.data(), &error);
    cl_mem ib_17 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray9) * cust_size, loadedData->C_ZIP.data(), &error);
    cl_mem ib_18 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * cust_size, loadedData->C_PHONE.data(), &error);
    cl_mem ib_19 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray10) * cust_size, loadedData->C_SINCE.data(), &error);
    cl_mem ib_20 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * cust_size, loadedData->C_MIDDLE.data(), &error);
    cl_mem ib_21 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray30) * cust_size, loadedData->C_DATA.data(), &error);

    cl_mem ob_1 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_ID.data(), &error);
    cl_mem ob_2 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_D_ID.data(), &error);
    cl_mem ob_3 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_W_ID.data(), &error);
    cl_mem ob_4 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), &error);
    cl_mem ob_5 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), &error);
    cl_mem ob_6 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_DISCOUNT.data(), &error);
    cl_mem ob_7 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_CREDIT_LIM.data(), &error);
    cl_mem ob_8 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_BALANCE.data(), &error);
    cl_mem ob_9 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), &error);
    cl_mem ob_10 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_LAST.data(), &error);
    cl_mem ob_11 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_FIRST.data(), &error);
    cl_mem ob_12 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_CREDIT.data(), &error);
    cl_mem ob_13 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_1.data(), &error);
    cl_mem ob_14 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_STREET_2.data(), &error);
    cl_mem ob_15 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_CITY.data(), &error);
    cl_mem ob_16 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_STATE.data(), &error);
    cl_mem ob_17 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray9) * queryCount, C_ZIP.data(), &error);
    cl_mem ob_18 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray20) * queryCount, C_PHONE.data(), &error);
    cl_mem ob_19 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray10) * queryCount, C_SINCE.data(), &error);
    cl_mem ob_20 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray2) * queryCount, C_MIDDLE.data(), &error);
    cl_mem ob_21 = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(charArray30) * queryCount, C_DATA.data(), &error);

    clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &ib_1);
    clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &ob_1);
    clSetKernelArg(kernel[0], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &ib_2);
    clSetKernelArg(kernel[1], 1, sizeof(cl_mem), &ob_2);
    clSetKernelArg(kernel[1], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[2], 0, sizeof(cl_mem), &ib_3);
    clSetKernelArg(kernel[2], 1, sizeof(cl_mem), &ob_3);
    clSetKernelArg(kernel[2], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[3], 0, sizeof(cl_mem), &ib_4);
    clSetKernelArg(kernel[3], 1, sizeof(cl_mem), &ob_4);
    clSetKernelArg(kernel[3], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[4], 0, sizeof(cl_mem), &ib_5);
    clSetKernelArg(kernel[4], 1, sizeof(cl_mem), &ob_5);
    clSetKernelArg(kernel[4], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[5], 0, sizeof(cl_mem), &ib_6);
    clSetKernelArg(kernel[5], 1, sizeof(cl_mem), &ob_6);
    clSetKernelArg(kernel[5], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[6], 0, sizeof(cl_mem), &ib_7);
    clSetKernelArg(kernel[6], 1, sizeof(cl_mem), &ob_7);
    clSetKernelArg(kernel[6], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[7], 0, sizeof(cl_mem), &ib_8);
    clSetKernelArg(kernel[7], 1, sizeof(cl_mem), &ob_8);
    clSetKernelArg(kernel[7], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[8], 0, sizeof(cl_mem), &ib_9);
    clSetKernelArg(kernel[8], 1, sizeof(cl_mem), &ob_9);
    clSetKernelArg(kernel[8], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[9], 0, sizeof(cl_mem), &ib_10);
    clSetKernelArg(kernel[9], 1, sizeof(cl_mem), &ob_10);
    clSetKernelArg(kernel[9], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[10], 0, sizeof(cl_mem), &ib_11);
    clSetKernelArg(kernel[10], 1, sizeof(cl_mem), &ob_11);
    clSetKernelArg(kernel[10], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[11], 0, sizeof(cl_mem), &ib_12);
    clSetKernelArg(kernel[11], 1, sizeof(cl_mem), &ob_12);
    clSetKernelArg(kernel[11], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[12], 0, sizeof(cl_mem), &ib_13);
    clSetKernelArg(kernel[12], 1, sizeof(cl_mem), &ob_13);
    clSetKernelArg(kernel[12], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[13], 0, sizeof(cl_mem), &ib_14);
    clSetKernelArg(kernel[13], 1, sizeof(cl_mem), &ob_14);
    clSetKernelArg(kernel[13], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[14], 0, sizeof(cl_mem), &ib_15);
    clSetKernelArg(kernel[14], 1, sizeof(cl_mem), &ob_15);
    clSetKernelArg(kernel[14], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[15], 0, sizeof(cl_mem), &ib_16);
    clSetKernelArg(kernel[15], 1, sizeof(cl_mem), &ob_16);
    clSetKernelArg(kernel[15], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[16], 0, sizeof(cl_mem), &ib_17);
    clSetKernelArg(kernel[16], 1, sizeof(cl_mem), &ob_17);
    clSetKernelArg(kernel[16], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[17], 0, sizeof(cl_mem), &ib_18);
    clSetKernelArg(kernel[17], 1, sizeof(cl_mem), &ob_18);
    clSetKernelArg(kernel[17], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[18], 0, sizeof(cl_mem), &ib_19);
    clSetKernelArg(kernel[18], 1, sizeof(cl_mem), &ob_19);
    clSetKernelArg(kernel[18], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[19], 0, sizeof(cl_mem), &ib_20);
    clSetKernelArg(kernel[19], 1, sizeof(cl_mem), &ob_20);
    clSetKernelArg(kernel[19], 2, sizeof(cl_mem), &ib_c);

    clSetKernelArg(kernel[20], 0, sizeof(cl_mem), &ib_21);
    clSetKernelArg(kernel[20], 1, sizeof(cl_mem), &ob_21);
    clSetKernelArg(kernel[20], 2, sizeof(cl_mem), &ib_c);
    
    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {queryCount};

    unsigned long kernelTime = 0;
    
    cl_event event[21];
    for (int i = 0; i < 21; i++) {
        CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);       
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }
    
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[0], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[1], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[2], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[3], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[4], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[5], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[6], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[7], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[8], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[9], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[10], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[11], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[12], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[13], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[14], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[15], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[16], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[17], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[18], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[19], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));
    //CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[20], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, nullptr));

    clFinish(queueCPU);


    CheckError(clEnqueueReadBuffer(queueCPU, ob_1, CL_TRUE, 0, sizeof(int)*queryCount, C_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_2, CL_TRUE, 0, sizeof(int)*queryCount, C_D_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_3, CL_TRUE, 0, sizeof(int)*queryCount, C_W_ID.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_4, CL_TRUE, 0, sizeof(int)*queryCount, C_PAYMENT_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_5, CL_TRUE, 0, sizeof(int)*queryCount, C_DELIVERY_CNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_6, CL_TRUE, 0, sizeof(float)*queryCount, C_DISCOUNT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_7, CL_TRUE, 0, sizeof(float)*queryCount, C_CREDIT_LIM.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_8, CL_TRUE, 0, sizeof(float)*queryCount, C_BALANCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_9, CL_TRUE, 0, sizeof(float)*queryCount, C_YTD_PAYMENT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_10, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_LAST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_11, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_FIRST.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_12, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_CREDIT.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_13, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_1.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_14, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_STREET_2.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_15, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_CITY.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_16, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_STATE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_17, CL_TRUE, 0, sizeof(charArray9) * queryCount, C_ZIP.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_18, CL_TRUE, 0, sizeof(charArray20) * queryCount, C_PHONE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_19, CL_TRUE, 0, sizeof(charArray10) * queryCount, C_SINCE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_20, CL_TRUE, 0, sizeof(charArray2) * queryCount, C_MIDDLE.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_21, CL_TRUE, 0, sizeof(charArray30) * queryCount, C_DATA.data(), 0, nullptr, nullptr));
    
    clReleaseCommandQueue(queueCPU);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;

    clReleaseProgram(CPUprogram_float_select);
    clReleaseProgram(CPUprogram_int_select);
    clReleaseProgram(CPUprogram_char2_select);
    clReleaseProgram(CPUprogram_char9_select);
    clReleaseProgram(CPUprogram_char10_select);
    clReleaseProgram(CPUprogram_char20_select);
    clReleaseProgram(CPUprogram_char30_select);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }

    clReleaseMemObject(ib_c);

    clReleaseMemObject(ib_1);
    clReleaseMemObject(ib_2);
    clReleaseMemObject(ib_3);
    clReleaseMemObject(ib_4);
    clReleaseMemObject(ib_5);
    clReleaseMemObject(ib_6);
    clReleaseMemObject(ib_7);
    clReleaseMemObject(ib_8);
    clReleaseMemObject(ib_9);
    clReleaseMemObject(ib_10);
    clReleaseMemObject(ib_11);
    clReleaseMemObject(ib_12);
    clReleaseMemObject(ib_13);
    clReleaseMemObject(ib_14);
    clReleaseMemObject(ib_15);
    clReleaseMemObject(ib_16);
    clReleaseMemObject(ib_17);
    clReleaseMemObject(ib_18);
    clReleaseMemObject(ib_19);
    clReleaseMemObject(ib_20);
    clReleaseMemObject(ib_21);

    clReleaseMemObject(ob_1);
    clReleaseMemObject(ob_2);
    clReleaseMemObject(ob_3);
    clReleaseMemObject(ob_4);
    clReleaseMemObject(ob_5);
    clReleaseMemObject(ob_6);
    clReleaseMemObject(ob_7);
    clReleaseMemObject(ob_8);
    clReleaseMemObject(ob_9);
    clReleaseMemObject(ob_10);
    clReleaseMemObject(ob_11);
    clReleaseMemObject(ob_12);
    clReleaseMemObject(ob_13);
    clReleaseMemObject(ob_14);
    clReleaseMemObject(ob_15);
    clReleaseMemObject(ob_16);
    clReleaseMemObject(ob_17);
    clReleaseMemObject(ob_18);
    clReleaseMemObject(ob_19);
    clReleaseMemObject(ob_20);
    clReleaseMemObject(ob_21);
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}

double OpenCLExecutionColumnStore::NewOrderColumnStoreGPU(cl_context GPUcontext,
        cl_uint GPUdeviceIdCount, std::vector<cl_device_id> GPUdeviceIds, int queryCount, InputDataNewOrderColumnStore inputData, bool includeTransferTime) {

    int OL_count = inputData.total_order_lines;

    //Order
    vector<int> o_w_id(queryCount);
    vector<int> o_d_id(queryCount);
    vector<int> o_c_id(queryCount);
    vector<int> o_id(queryCount);
    vector<int> o_carrier_id(queryCount);
    vector<int> o_ol_cnt(queryCount);
    vector<int> o_all_local(queryCount);
    vector<charArray10> o_entry_d(queryCount);

    //New Order
    vector<int> no_w_id(queryCount);
    vector<int> no_d_id(queryCount);
    vector<int> no_id(queryCount);

    //Order Line
    vector<charArray30> ol_dist_info(OL_count);
    vector<int> ol_o_id(OL_count);
    vector<int> ol_d_id(OL_count);
    vector<int> ol_w_id(OL_count);
    vector<int> ol_number(OL_count);
    vector<int> ol_item_id(OL_count);
    vector<charArray20> ol_delivery_d(OL_count);
    vector<float> ol_amount(OL_count);
    vector<int> ol_supply_w_id(OL_count);
    vector<int> ol_ol_number(OL_count);

    cl_int error = CL_SUCCESS;

    cl_program GPUprogram_char2_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char2_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_char9_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char9_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_char10_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char10_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_char20_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char20_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_char30_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_char30_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_int_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_int_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program GPUprogram_float_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_insert.cl"), GPUcontext);
    CheckError(clBuildProgram(GPUprogram_float_insert, GPUdeviceIdCount, GPUdeviceIds.data(), nullptr, nullptr, nullptr));

    float timeInSeconds;

    cl_kernel kernel[21];

    for (int i = 0; i < 7; i++) {
        kernel[i] = clCreateKernel(GPUprogram_int_insert, "op_int_insert", &error);
    }
    kernel[7] = clCreateKernel(GPUprogram_char10_insert, "op_char10_insert", &error);

    for (int i = 8; i < 11; i++) {
        kernel[i] = clCreateKernel(GPUprogram_int_insert, "op_int_insert", &error);
    }

    kernel[11] = clCreateKernel(GPUprogram_char30_insert, "op_char30_insert", &error);

    for (int i = 12; i < 19; i++) {
        kernel[i] = clCreateKernel(GPUprogram_int_insert, "op_int_insert", &error);
    }

    kernel[19] = clCreateKernel(GPUprogram_float_insert, "op_float_insert", &error);
    kernel[20] = clCreateKernel(GPUprogram_char20_insert, "op_char20_insert", &error);


    cl_mem ib_o_w_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_w_id.data(), &error);
    cl_mem ib_o_d_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_d_id.data(), &error);
    cl_mem ib_o_c_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_c_id.data(), &error);
    cl_mem ib_o_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_id.data(), &error);
    cl_mem ib_o_carrier_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_carrier_id.data(), &error);
    cl_mem ib_o_ol_cnt = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_ol_cnt.data(), &error);
    cl_mem ib_o_all_local = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_all_local.data(), &error);
    cl_mem ib_o_entry_d = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (charArray10) * queryCount, inputData.o_entry_d.data(), &error);


    cl_mem ib_ol_dist_info = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (charArray30) * OL_count, inputData.ol_dist_info.data(), &error);
    cl_mem ib_ol_o_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_o_id.data(), &error);
    cl_mem ib_ol_d_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_d_id.data(), &error);
    cl_mem ib_ol_w_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_w_id.data(), &error);
    cl_mem ib_ol_number = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*OL_count, inputData.ol_number.data(), &error);
    cl_mem ib_ol_item_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_item_id.data(), &error);
    cl_mem ib_ol_delivery_d = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (charArray20) * OL_count, inputData.ol_delivery_d.data(), &error);
    cl_mem ib_ol_amount = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (float)*OL_count, inputData.ol_amount.data(), &error);
    cl_mem ib_ol_supply_w_id = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_supply_w_id.data(), &error);
    cl_mem ib_ol_ol_number = clCreateBuffer(GPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_ol_number.data(), &error);

    cl_mem ob_o_w_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, o_w_id.data(), &error);
    cl_mem ob_o_d_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, o_d_id.data(), &error);
    cl_mem ob_o_c_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, o_c_id.data(), &error);
    cl_mem ob_o_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_id.data(), &error);
    cl_mem ob_o_carrier_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_carrier_id.data(), &error);
    cl_mem ob_o_ol_cnt = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_ol_cnt.data(), &error);
    cl_mem ob_o_all_local = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_all_local.data(), &error);
    cl_mem ob_o_entry_d = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (charArray10) * (queryCount), o_entry_d.data(), &error);

    cl_mem ob_no_w_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), no_w_id.data(), &error);
    cl_mem ob_no_d_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), no_d_id.data(), &error);
    cl_mem ob_no_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), no_id.data(), &error);

    cl_mem ob_ol_dist_info = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (charArray30) * OL_count, ol_dist_info.data(), &error);
    cl_mem ob_ol_o_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_o_id.data(), &error);
    cl_mem ob_ol_d_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_d_id.data(), &error);
    cl_mem ob_ol_w_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_w_id.data(), &error);
    cl_mem ob_ol_number = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_number.data(), &error);
    cl_mem ob_ol_item_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_item_id.data(), &error);
    cl_mem ob_ol_delivery_d = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (charArray20) * OL_count, ol_delivery_d.data(), &error);
    cl_mem ob_ol_amount = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (float)*OL_count, ol_amount.data(), &error);
    cl_mem ob_ol_supply_w_id = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_supply_w_id.data(), &error);
    cl_mem ob_ol_ol_number = clCreateBuffer(GPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_ol_number.data(), &error);

    clSetKernelArg(kernel[0], 0, sizeof (cl_mem), &ib_o_w_id);
    clSetKernelArg(kernel[0], 1, sizeof (cl_mem), &ob_o_w_id);
    clSetKernelArg(kernel[1], 0, sizeof (cl_mem), &ib_o_d_id);
    clSetKernelArg(kernel[1], 1, sizeof (cl_mem), &ob_o_d_id);
    clSetKernelArg(kernel[2], 0, sizeof (cl_mem), &ib_o_c_id);
    clSetKernelArg(kernel[2], 1, sizeof (cl_mem), &ob_o_c_id);
    clSetKernelArg(kernel[3], 0, sizeof (cl_mem), &ib_o_id);
    clSetKernelArg(kernel[3], 1, sizeof (cl_mem), &ob_o_id);
    clSetKernelArg(kernel[4], 0, sizeof (cl_mem), &ib_o_carrier_id);
    clSetKernelArg(kernel[4], 1, sizeof (cl_mem), &ob_o_carrier_id);
    clSetKernelArg(kernel[5], 0, sizeof (cl_mem), &ib_o_ol_cnt);
    clSetKernelArg(kernel[5], 1, sizeof (cl_mem), &ob_o_ol_cnt);
    clSetKernelArg(kernel[6], 0, sizeof (cl_mem), &ib_o_all_local);
    clSetKernelArg(kernel[6], 1, sizeof (cl_mem), &ob_o_all_local);

    clSetKernelArg(kernel[7], 0, sizeof (cl_mem), &ib_o_entry_d);
    clSetKernelArg(kernel[7], 1, sizeof (cl_mem), &ob_o_entry_d);

    clSetKernelArg(kernel[8], 0, sizeof (cl_mem), &ib_o_w_id);
    clSetKernelArg(kernel[8], 1, sizeof (cl_mem), &ob_no_w_id);
    clSetKernelArg(kernel[9], 0, sizeof (cl_mem), &ib_o_d_id);
    clSetKernelArg(kernel[9], 1, sizeof (cl_mem), &ob_no_d_id);
    clSetKernelArg(kernel[10], 0, sizeof (cl_mem), &ib_o_id);
    clSetKernelArg(kernel[10], 1, sizeof (cl_mem), &ob_no_id);

    clSetKernelArg(kernel[11], 0, sizeof (cl_mem), &ib_ol_dist_info);
    clSetKernelArg(kernel[11], 1, sizeof (cl_mem), &ob_ol_dist_info);

    clSetKernelArg(kernel[12], 0, sizeof (cl_mem), &ib_ol_o_id);
    clSetKernelArg(kernel[12], 1, sizeof (cl_mem), &ob_ol_o_id);

    clSetKernelArg(kernel[13], 0, sizeof (cl_mem), &ib_ol_d_id);
    clSetKernelArg(kernel[13], 1, sizeof (cl_mem), &ob_ol_d_id);

    clSetKernelArg(kernel[14], 0, sizeof (cl_mem), &ib_ol_w_id);
    clSetKernelArg(kernel[14], 1, sizeof (cl_mem), &ob_ol_w_id);

    clSetKernelArg(kernel[15], 0, sizeof (cl_mem), &ib_ol_number);
    clSetKernelArg(kernel[15], 1, sizeof (cl_mem), &ob_ol_number);

    clSetKernelArg(kernel[16], 0, sizeof (cl_mem), &ib_ol_item_id);
    clSetKernelArg(kernel[16], 1, sizeof (cl_mem), &ob_ol_item_id);

    clSetKernelArg(kernel[17], 0, sizeof (cl_mem), &ib_ol_supply_w_id);
    clSetKernelArg(kernel[17], 1, sizeof (cl_mem), &ob_ol_supply_w_id);

    clSetKernelArg(kernel[18], 0, sizeof (cl_mem), &ib_ol_ol_number);
    clSetKernelArg(kernel[18], 1, sizeof (cl_mem), &ob_ol_ol_number);

    clSetKernelArg(kernel[19], 0, sizeof (cl_mem), &ib_ol_amount);
    clSetKernelArg(kernel[19], 1, sizeof (cl_mem), &ob_ol_amount);

    clSetKernelArg(kernel[20], 0, sizeof (cl_mem), &ib_ol_delivery_d);
    clSetKernelArg(kernel[20], 1, sizeof (cl_mem), &ob_ol_delivery_d);

    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueGPU = clCreateCommandQueue(GPUcontext, GPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {queryCount};
    size_t globalWorkSizeOL[] = {OL_count};

    unsigned long kernelTime = 0;

    cl_event event[21];
    for (int i = 0; i < 11; i++) {
        CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }
    for (int i = 11; i < 21; i++) {
        CheckError(clEnqueueNDRangeKernel(queueGPU, kernel[i], 1, nullptr, globalWorkSizeOL, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }


    clFinish(queueGPU);

    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_w_id, CL_TRUE, 0, sizeof (int)*queryCount, o_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_d_id, CL_TRUE, 0, sizeof (int)*queryCount, o_d_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_c_id, CL_TRUE, 0, sizeof (int)*queryCount, o_c_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_id, CL_TRUE, 0, sizeof (int)*queryCount, o_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_carrier_id, CL_TRUE, 0, sizeof (int)*queryCount, o_carrier_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_ol_cnt, CL_TRUE, 0, sizeof (int)*queryCount, o_ol_cnt.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_all_local, CL_TRUE, 0, sizeof (int)*queryCount, o_all_local.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_o_entry_d, CL_TRUE, 0, sizeof (charArray10) * queryCount, o_entry_d.data(), 0, nullptr, nullptr));

    CheckError(clEnqueueReadBuffer(queueGPU, ob_no_w_id, CL_TRUE, 0, sizeof (int)*queryCount, no_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_no_d_id, CL_TRUE, 0, sizeof (int)*queryCount, no_d_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_no_id, CL_TRUE, 0, sizeof (int)*queryCount, no_id.data(), 0, nullptr, nullptr));

    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_dist_info, CL_TRUE, 0, sizeof (charArray30) * OL_count, ol_dist_info.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_o_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_o_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_d_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_d_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_w_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_number, CL_TRUE, 0, sizeof (int)*OL_count, ol_number.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_item_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_item_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_amount, CL_TRUE, 0, sizeof (float)*OL_count, ol_amount.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_supply_w_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_supply_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_ol_number, CL_TRUE, 0, sizeof (int)*OL_count, ol_ol_number.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueGPU, ob_ol_delivery_d, CL_TRUE, 0, sizeof (charArray20) * OL_count, ol_delivery_d.data(), 0, nullptr, nullptr));
    clReleaseCommandQueue(queueGPU);

    clReleaseMemObject(ib_o_w_id);
    clReleaseMemObject(ib_o_d_id);
    clReleaseMemObject(ib_o_c_id);
    clReleaseMemObject(ib_o_id);
    clReleaseMemObject(ib_o_carrier_id);
    clReleaseMemObject(ib_o_ol_cnt);
    clReleaseMemObject(ib_o_all_local);
    clReleaseMemObject(ib_o_entry_d);

    clReleaseMemObject(ib_ol_dist_info);
    clReleaseMemObject(ib_ol_o_id);
    clReleaseMemObject(ib_ol_d_id);
    clReleaseMemObject(ib_ol_w_id);
    clReleaseMemObject(ib_ol_number);
    clReleaseMemObject(ib_ol_item_id);
    clReleaseMemObject(ib_ol_delivery_d);
    clReleaseMemObject(ib_ol_amount);
    clReleaseMemObject(ib_ol_supply_w_id);
    clReleaseMemObject(ib_ol_ol_number);

    clReleaseMemObject(ob_o_w_id);
    clReleaseMemObject(ob_o_d_id);
    clReleaseMemObject(ob_o_c_id);
    clReleaseMemObject(ob_o_id);

    clReleaseMemObject(ob_o_carrier_id);
    clReleaseMemObject(ob_o_ol_cnt);
    clReleaseMemObject(ob_o_all_local);
    clReleaseMemObject(ob_o_entry_d);
    clReleaseMemObject(ob_no_w_id);
    clReleaseMemObject(ob_no_d_id);
    clReleaseMemObject(ob_no_id);
    clReleaseMemObject(ob_ol_dist_info);
    clReleaseMemObject(ob_ol_o_id);
    clReleaseMemObject(ob_ol_d_id);
    clReleaseMemObject(ob_ol_delivery_d);
    clReleaseMemObject(ob_ol_w_id);
    clReleaseMemObject(ob_ol_number);
    clReleaseMemObject(ob_ol_item_id);
    clReleaseMemObject(ob_ol_amount);
    clReleaseMemObject(ob_ol_supply_w_id);

    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;



    clReleaseProgram(GPUprogram_int_insert);
    clReleaseProgram(GPUprogram_char2_insert);
    clReleaseProgram(GPUprogram_char9_insert);
    clReleaseProgram(GPUprogram_char10_insert);
    clReleaseProgram(GPUprogram_char20_insert);
    clReleaseProgram(GPUprogram_char30_insert);
    clReleaseProgram(GPUprogram_float_insert);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }

    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}


double OpenCLExecutionColumnStore::NewOrderColumnStoreCPU(cl_context CPUcontext,
        cl_uint CPUdeviceIdCount, std::vector<cl_device_id> CPUdeviceIds, int queryCount, InputDataNewOrderColumnStore inputData, bool includeTransferTime) {

    int OL_count = inputData.total_order_lines;

    //Order
    vector<int> o_w_id(queryCount);
    vector<int> o_d_id(queryCount);
    vector<int> o_c_id(queryCount);
    vector<int> o_id(queryCount);
    vector<int> o_carrier_id(queryCount);
    vector<int> o_ol_cnt(queryCount);
    vector<int> o_all_local(queryCount);
    vector<charArray10> o_entry_d(queryCount);

    //New Order
    vector<int> no_w_id(queryCount);
    vector<int> no_d_id(queryCount);
    vector<int> no_id(queryCount);

    //Order Line
    vector<charArray30> ol_dist_info(OL_count);
    vector<int> ol_o_id(OL_count);
    vector<int> ol_d_id(OL_count);
    vector<int> ol_w_id(OL_count);
    vector<int> ol_number(OL_count);
    vector<int> ol_item_id(OL_count);
    vector<charArray20> ol_delivery_d(OL_count);
    vector<float> ol_amount(OL_count);
    vector<int> ol_supply_w_id(OL_count);
    vector<int> ol_ol_number(OL_count);

    cl_int error = CL_SUCCESS;

    cl_program CPUprogram_char2_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char2_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char2_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_char9_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char9_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char9_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_char10_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char10_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char10_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_char20_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char20_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char20_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_char30_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_char30_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_char30_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_int_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_int_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_int_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    cl_program CPUprogram_float_insert = CreateProgram(LoadKernel("/home/arefeva/NBP/data/op_float_insert.cl"), CPUcontext);
    CheckError(clBuildProgram(CPUprogram_float_insert, CPUdeviceIdCount, CPUdeviceIds.data(), nullptr, nullptr, nullptr));

    float timeInSeconds;

    cl_kernel kernel[21];

    for (int i = 0; i < 7; i++) {
        kernel[i] = clCreateKernel(CPUprogram_int_insert, "op_int_insert", &error);
    }
    kernel[7] = clCreateKernel(CPUprogram_char10_insert, "op_char10_insert", &error);

    for (int i = 8; i < 11; i++) {
        kernel[i] = clCreateKernel(CPUprogram_int_insert, "op_int_insert", &error);
    }

    kernel[11] = clCreateKernel(CPUprogram_char30_insert, "op_char30_insert", &error);

    for (int i = 12; i < 19; i++) {
        kernel[i] = clCreateKernel(CPUprogram_int_insert, "op_int_insert", &error);
    }

    kernel[19] = clCreateKernel(CPUprogram_float_insert, "op_float_insert", &error);
    kernel[20] = clCreateKernel(CPUprogram_char20_insert, "op_char20_insert", &error);


    cl_mem ib_o_w_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_w_id.data(), &error);
    cl_mem ib_o_d_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_d_id.data(), &error);
    cl_mem ib_o_c_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_c_id.data(), &error);
    cl_mem ib_o_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_id.data(), &error);
    cl_mem ib_o_carrier_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_carrier_id.data(), &error);
    cl_mem ib_o_ol_cnt = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_ol_cnt.data(), &error);
    cl_mem ib_o_all_local = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, inputData.o_all_local.data(), &error);
    cl_mem ib_o_entry_d = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (charArray10) * queryCount, inputData.o_entry_d.data(), &error);


    cl_mem ib_ol_dist_info = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (charArray30) * OL_count, inputData.ol_dist_info.data(), &error);
    cl_mem ib_ol_o_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_o_id.data(), &error);
    cl_mem ib_ol_d_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_d_id.data(), &error);
    cl_mem ib_ol_w_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_w_id.data(), &error);
    cl_mem ib_ol_number = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)*OL_count, inputData.ol_number.data(), &error);
    cl_mem ib_ol_item_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_item_id.data(), &error);
    cl_mem ib_ol_delivery_d = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (charArray20) * OL_count, inputData.ol_delivery_d.data(), &error);
    cl_mem ib_ol_amount = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (float)*OL_count, inputData.ol_amount.data(), &error);
    cl_mem ib_ol_supply_w_id = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_supply_w_id.data(), &error);
    cl_mem ib_ol_ol_number = clCreateBuffer(CPUcontext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, inputData.ol_ol_number.data(), &error);

    cl_mem ob_o_w_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, o_w_id.data(), &error);
    cl_mem ob_o_d_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, o_d_id.data(), &error);
    cl_mem ob_o_c_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*queryCount, o_c_id.data(), &error);
    cl_mem ob_o_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_id.data(), &error);
    cl_mem ob_o_carrier_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_carrier_id.data(), &error);
    cl_mem ob_o_ol_cnt = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_ol_cnt.data(), &error);
    cl_mem ob_o_all_local = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), o_all_local.data(), &error);
    cl_mem ob_o_entry_d = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (charArray10) * (queryCount), o_entry_d.data(), &error);

    cl_mem ob_no_w_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), no_w_id.data(), &error);
    cl_mem ob_no_d_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), no_d_id.data(), &error);
    cl_mem ob_no_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int) * (queryCount), no_id.data(), &error);

    cl_mem ob_ol_dist_info = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (charArray30) * OL_count, ol_dist_info.data(), &error);
    cl_mem ob_ol_o_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_o_id.data(), &error);
    cl_mem ob_ol_d_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_d_id.data(), &error);
    cl_mem ob_ol_w_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_w_id.data(), &error);
    cl_mem ob_ol_number = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_number.data(), &error);
    cl_mem ob_ol_item_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_item_id.data(), &error);
    cl_mem ob_ol_delivery_d = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (charArray20) * OL_count, ol_delivery_d.data(), &error);
    cl_mem ob_ol_amount = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (float)*OL_count, ol_amount.data(), &error);
    cl_mem ob_ol_supply_w_id = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_supply_w_id.data(), &error);
    cl_mem ob_ol_ol_number = clCreateBuffer(CPUcontext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof (int)*OL_count, ol_ol_number.data(), &error);

    clSetKernelArg(kernel[0], 0, sizeof (cl_mem), &ib_o_w_id);
    clSetKernelArg(kernel[0], 1, sizeof (cl_mem), &ob_o_w_id);
    clSetKernelArg(kernel[1], 0, sizeof (cl_mem), &ib_o_d_id);
    clSetKernelArg(kernel[1], 1, sizeof (cl_mem), &ob_o_d_id);
    clSetKernelArg(kernel[2], 0, sizeof (cl_mem), &ib_o_c_id);
    clSetKernelArg(kernel[2], 1, sizeof (cl_mem), &ob_o_c_id);
    clSetKernelArg(kernel[3], 0, sizeof (cl_mem), &ib_o_id);
    clSetKernelArg(kernel[3], 1, sizeof (cl_mem), &ob_o_id);
    clSetKernelArg(kernel[4], 0, sizeof (cl_mem), &ib_o_carrier_id);
    clSetKernelArg(kernel[4], 1, sizeof (cl_mem), &ob_o_carrier_id);
    clSetKernelArg(kernel[5], 0, sizeof (cl_mem), &ib_o_ol_cnt);
    clSetKernelArg(kernel[5], 1, sizeof (cl_mem), &ob_o_ol_cnt);
    clSetKernelArg(kernel[6], 0, sizeof (cl_mem), &ib_o_all_local);
    clSetKernelArg(kernel[6], 1, sizeof (cl_mem), &ob_o_all_local);

    clSetKernelArg(kernel[7], 0, sizeof (cl_mem), &ib_o_entry_d);
    clSetKernelArg(kernel[7], 1, sizeof (cl_mem), &ob_o_entry_d);

    clSetKernelArg(kernel[8], 0, sizeof (cl_mem), &ib_o_w_id);
    clSetKernelArg(kernel[8], 1, sizeof (cl_mem), &ob_no_w_id);
    clSetKernelArg(kernel[9], 0, sizeof (cl_mem), &ib_o_d_id);
    clSetKernelArg(kernel[9], 1, sizeof (cl_mem), &ob_no_d_id);
    clSetKernelArg(kernel[10], 0, sizeof (cl_mem), &ib_o_id);
    clSetKernelArg(kernel[10], 1, sizeof (cl_mem), &ob_no_id);

    clSetKernelArg(kernel[11], 0, sizeof (cl_mem), &ib_ol_dist_info);
    clSetKernelArg(kernel[11], 1, sizeof (cl_mem), &ob_ol_dist_info);

    clSetKernelArg(kernel[12], 0, sizeof (cl_mem), &ib_ol_o_id);
    clSetKernelArg(kernel[12], 1, sizeof (cl_mem), &ob_ol_o_id);

    clSetKernelArg(kernel[13], 0, sizeof (cl_mem), &ib_ol_d_id);
    clSetKernelArg(kernel[13], 1, sizeof (cl_mem), &ob_ol_d_id);

    clSetKernelArg(kernel[14], 0, sizeof (cl_mem), &ib_ol_w_id);
    clSetKernelArg(kernel[14], 1, sizeof (cl_mem), &ob_ol_w_id);

    clSetKernelArg(kernel[15], 0, sizeof (cl_mem), &ib_ol_number);
    clSetKernelArg(kernel[15], 1, sizeof (cl_mem), &ob_ol_number);

    clSetKernelArg(kernel[16], 0, sizeof (cl_mem), &ib_ol_item_id);
    clSetKernelArg(kernel[16], 1, sizeof (cl_mem), &ob_ol_item_id);

    clSetKernelArg(kernel[17], 0, sizeof (cl_mem), &ib_ol_supply_w_id);
    clSetKernelArg(kernel[17], 1, sizeof (cl_mem), &ob_ol_supply_w_id);

    clSetKernelArg(kernel[18], 0, sizeof (cl_mem), &ib_ol_ol_number);
    clSetKernelArg(kernel[18], 1, sizeof (cl_mem), &ob_ol_ol_number);

    clSetKernelArg(kernel[19], 0, sizeof (cl_mem), &ib_ol_amount);
    clSetKernelArg(kernel[19], 1, sizeof (cl_mem), &ob_ol_amount);

    clSetKernelArg(kernel[20], 0, sizeof (cl_mem), &ib_ol_delivery_d);
    clSetKernelArg(kernel[20], 1, sizeof (cl_mem), &ob_ol_delivery_d);

    auto startTime = std::chrono::high_resolution_clock::now();

    cl_command_queue queueCPU = clCreateCommandQueue(CPUcontext, CPUdeviceIds[0], CL_QUEUE_PROFILING_ENABLE, &error);

    size_t globalWorkSize[] = {queryCount};
    size_t globalWorkSizeOL[] = {OL_count};

    unsigned long kernelTime = 0;

    cl_event event[21];
    for (int i = 0; i < 11; i++) {
        CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[i], 1, nullptr, globalWorkSize, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }
    for (int i = 11; i < 21; i++) {
        CheckError(clEnqueueNDRangeKernel(queueCPU, kernel[i], 1, nullptr, globalWorkSizeOL, nullptr, 0, nullptr, &event[i]));
        clWaitForEvents(1, &event[i]);
        unsigned long start = 0;
        unsigned long end = 0;
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
        clGetEventProfilingInfo(event[i],CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
        kernelTime += end - start;
    }


    clFinish(queueCPU);

    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_w_id, CL_TRUE, 0, sizeof (int)*queryCount, o_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_d_id, CL_TRUE, 0, sizeof (int)*queryCount, o_d_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_c_id, CL_TRUE, 0, sizeof (int)*queryCount, o_c_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_id, CL_TRUE, 0, sizeof (int)*queryCount, o_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_carrier_id, CL_TRUE, 0, sizeof (int)*queryCount, o_carrier_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_ol_cnt, CL_TRUE, 0, sizeof (int)*queryCount, o_ol_cnt.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_all_local, CL_TRUE, 0, sizeof (int)*queryCount, o_all_local.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_o_entry_d, CL_TRUE, 0, sizeof (charArray10) * queryCount, o_entry_d.data(), 0, nullptr, nullptr));

    CheckError(clEnqueueReadBuffer(queueCPU, ob_no_w_id, CL_TRUE, 0, sizeof (int)*queryCount, no_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_no_d_id, CL_TRUE, 0, sizeof (int)*queryCount, no_d_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_no_id, CL_TRUE, 0, sizeof (int)*queryCount, no_id.data(), 0, nullptr, nullptr));

    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_dist_info, CL_TRUE, 0, sizeof (charArray30) * OL_count, ol_dist_info.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_o_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_o_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_d_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_d_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_w_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_number, CL_TRUE, 0, sizeof (int)*OL_count, ol_number.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_item_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_item_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_amount, CL_TRUE, 0, sizeof (float)*OL_count, ol_amount.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_supply_w_id, CL_TRUE, 0, sizeof (int)*OL_count, ol_supply_w_id.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_ol_number, CL_TRUE, 0, sizeof (int)*OL_count, ol_ol_number.data(), 0, nullptr, nullptr));
    CheckError(clEnqueueReadBuffer(queueCPU, ob_ol_delivery_d, CL_TRUE, 0, sizeof (charArray20) * OL_count, ol_delivery_d.data(), 0, nullptr, nullptr));
    clReleaseCommandQueue(queueCPU);

    clReleaseMemObject(ib_o_w_id);
    clReleaseMemObject(ib_o_d_id);
    clReleaseMemObject(ib_o_c_id);
    clReleaseMemObject(ib_o_id);
    clReleaseMemObject(ib_o_carrier_id);
    clReleaseMemObject(ib_o_ol_cnt);
    clReleaseMemObject(ib_o_all_local);
    clReleaseMemObject(ib_o_entry_d);

    clReleaseMemObject(ib_ol_dist_info);
    clReleaseMemObject(ib_ol_o_id);
    clReleaseMemObject(ib_ol_d_id);
    clReleaseMemObject(ib_ol_w_id);
    clReleaseMemObject(ib_ol_number);
    clReleaseMemObject(ib_ol_item_id);
    clReleaseMemObject(ib_ol_delivery_d);
    clReleaseMemObject(ib_ol_amount);
    clReleaseMemObject(ib_ol_supply_w_id);
    clReleaseMemObject(ib_ol_ol_number);

    clReleaseMemObject(ob_o_w_id);
    clReleaseMemObject(ob_o_d_id);
    clReleaseMemObject(ob_o_c_id);
    clReleaseMemObject(ob_o_id);

    clReleaseMemObject(ob_o_carrier_id);
    clReleaseMemObject(ob_o_ol_cnt);
    clReleaseMemObject(ob_o_all_local);
    clReleaseMemObject(ob_o_entry_d);
    clReleaseMemObject(ob_no_w_id);
    clReleaseMemObject(ob_no_d_id);
    clReleaseMemObject(ob_no_id);
    clReleaseMemObject(ob_ol_dist_info);
    clReleaseMemObject(ob_ol_o_id);
    clReleaseMemObject(ob_ol_d_id);
    clReleaseMemObject(ob_ol_delivery_d);
    clReleaseMemObject(ob_ol_w_id);
    clReleaseMemObject(ob_ol_number);
    clReleaseMemObject(ob_ol_item_id);
    clReleaseMemObject(ob_ol_amount);
    clReleaseMemObject(ob_ol_supply_w_id);

    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;



    clReleaseProgram(CPUprogram_int_insert);
    clReleaseProgram(CPUprogram_char2_insert);
    clReleaseProgram(CPUprogram_char9_insert);
    clReleaseProgram(CPUprogram_char10_insert);
    clReleaseProgram(CPUprogram_char20_insert);
    clReleaseProgram(CPUprogram_char30_insert);
    clReleaseProgram(CPUprogram_float_insert);

    for (int i = 0; i < 21; i++) {
        clReleaseKernel(kernel[i]);
    }

    //auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    timeInSeconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    if (!includeTransferTime) return kernelTime/1000000.0;
    else return timeInSeconds/1000;
}









