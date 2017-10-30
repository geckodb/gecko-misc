
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "CL/cl.h"
#include "Row_Store_Data.h"
#include "QueryDataGenerator.h"
#include "OpenCLExecutionRowStore.h"
#include "OpenCLExecutionColumnStore.h"
//#include "TransactionsColumnStore.h"
//#include "TransactionsRowStore.h"
#include "Column_Store_Data.h"
#include "Row_Store_Import_Methods.h"
#include "Column_Store_Import_methods.h"
#include <cstdio>
#include <ctime>

char** rowStoreData;
ColumnStoreData columnStoreData;

ofstream outputFile;

cl_uint platformIdCount = 0;

cl_context GPUcontext;
cl_uint GPUdeviceIdCount = 0;
std::vector<cl_device_id> GPUdeviceIds(1);

cl_context CPUcontext;
cl_uint CPUdeviceIdCount = 0;
std::vector<cl_device_id> CPUdeviceIds(1);

QueryDataGenerator dataGenerator;

OpenCLExecutionRowStore queryExecutorRowStore;
OpenCLExecutionColumnStore queryExecutorColumnStore;
//TransactionsColumnStore transactionsColumnStore;
//TransactionsRowStore transactionsRowStore;


void ImportCSVData() {
    Row_Store_Import_Methods importRowStoreMethods;
    Column_Store_Import_methods importColumnStoreMethods;

        rowStoreData = new char*[9];
        importRowStoreMethods.ImportRowMajorData_District(rowStoreData, 0);
        importRowStoreMethods.ImportRowMajorData_Customer(rowStoreData, 1);
        importRowStoreMethods.ImportRowMajorData_History(rowStoreData, 2);
        importRowStoreMethods.ImportRowMajorData_Item(rowStoreData, 3);
        importRowStoreMethods.ImportRowMajorData_NewOrder(rowStoreData, 4);
        importRowStoreMethods.ImportRowMajorData_OrderLine(rowStoreData, 5);
        importRowStoreMethods.ImportRowMajorData_Stock(rowStoreData, 6);
        importRowStoreMethods.ImportRowMajorData_Warehouse(rowStoreData, 7);
        importRowStoreMethods.ImportRowMajorData_Orders(rowStoreData, 8);

        importColumnStoreMethods.ImportColumnMajorData_Customer(&columnStoreData);
        importColumnStoreMethods.ImportColumnMajorData_District(&columnStoreData);
        importColumnStoreMethods.ImportColumnMajorData_Warehouse(&columnStoreData);

        importColumnStoreMethods.ImportColumnMajorData_History(&columnStoreData);
        importColumnStoreMethods.ImportColumnMajorData_Item(&columnStoreData);
        importColumnStoreMethods.ImportColumnMajorData_NewOrder(&columnStoreData);
        importColumnStoreMethods.ImportColumnMajorData_OrderLine(&columnStoreData);
        importColumnStoreMethods.ImportColumnMajorData_Orders(&columnStoreData);
        importColumnStoreMethods.ImportColumnMajorData_Stock(&columnStoreData);
}

std::string GetPlatformName(cl_platform_id id) {
    size_t size = 0;
    clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, NULL, &size);

    std::string result;
    result.resize(size);
    clGetPlatformInfo(id, CL_PLATFORM_NAME, size,
            const_cast<char*> (result.data()), NULL);

    return result;
}

std::string GetDeviceName(cl_device_id id) {
    size_t size = 0;
    clGetDeviceInfo(id, CL_DEVICE_NAME, 0, NULL, &size);
    //clGetDeviceInfo(id, CL_DEVICE_VERSION, 0, NULL, &size);

    std::string result;
    result.resize(size);
    clGetDeviceInfo(id, CL_DEVICE_NAME, size, const_cast<char*> (result.data()), NULL);
    //clGetDeviceInfo(id, CL_DEVICE_VERSION, size, const_cast<char*> (result.data()), NULL);

    return result;
}

void CheckError(cl_int error) {
    if (error != CL_SUCCESS) {
        std::cerr << "OpenCL call failed with error " << error << std::endl;
        std::exit(1);
    }
}

int InitializeOpenCL() {

    clGetPlatformIDs(0, NULL, &platformIdCount);

    if (platformIdCount == 0) {
        std::cerr << "No OpenCL platform found" << std::endl;
        return 1;
    } else {
        std::cout << "Found " << platformIdCount << " platform(s)" << std::endl;
    }

    std::vector<cl_platform_id> platformIds(platformIdCount);
    clGetPlatformIDs(platformIdCount, platformIds.data(), NULL);

    for (cl_uint i = 0; i < platformIdCount; ++i) {
        std::cout << "\t (" << (i + 1) << ") : " << GetPlatformName(platformIds[i]) << std::endl;
    }



    clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, 0, NULL,
            &GPUdeviceIdCount);

    if (GPUdeviceIdCount == 0) {
        std::cerr << "No GPU devices found" << std::endl;
        return 1;
    } else {
        std::cout << "Found " << GPUdeviceIdCount << " device(s)" << std::endl;
    }


    clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, GPUdeviceIdCount,
            GPUdeviceIds.data(), NULL);

    for (cl_uint i = 0; i < GPUdeviceIdCount; ++i) {
        std::cout << "\t (" << (i + 1) << ") : " << GetDeviceName(GPUdeviceIds[i]) << std::endl;
    }

    // http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateContext.html
    const cl_context_properties GPUcontextProperties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (platformIds[0]),
        0, 0
    };

    cl_int error = CL_SUCCESS;
    GPUcontext = clCreateContext(GPUcontextProperties, GPUdeviceIdCount,
            GPUdeviceIds.data(), NULL, NULL, &error);
    CheckError(error);

    std::cout << "GPU Context created" << std::endl;


    clGetDeviceIDs(platformIds[1], CL_DEVICE_TYPE_ALL, 0, NULL,
            &CPUdeviceIdCount);

    if (CPUdeviceIdCount == 0) {
        std::cerr << "No CPU devices found" << std::endl;
        return 1;
    } else {
        std::cout << "Found " << CPUdeviceIdCount << " device(s)" << std::endl;
    }


    clGetDeviceIDs(platformIds[1], CL_DEVICE_TYPE_ALL, CPUdeviceIdCount,
            CPUdeviceIds.data(), NULL);

    for (cl_uint i = 0; i < CPUdeviceIdCount; ++i) {
        std::cout << "\t (" << (i + 1) << ") : " << GetDeviceName(CPUdeviceIds[i]) << std::endl;
    }

    // http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateContext.html
    const cl_context_properties CPUcontextProperties[] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (platformIds[1]),
        0, 0
    };

    //cl_int error = CL_SUCCESS;
    CPUcontext = clCreateContext(CPUcontextProperties, CPUdeviceIdCount,
            CPUdeviceIds.data(), NULL, NULL, &error);
    CheckError(error);

    std::cout << "CPU Context created" << std::endl;
    return 0;

}

int main() {

    outputFile.open("results.txt");

    /*char* test = (char*)malloc(203);
    cout << "0: " << (int*)read_customer(test, 0, 0) << endl;
    cout << (int*)read_customer(test, 0, 1) << endl;
    cout << "2: " << (int*)read_customer(test, 0, 2) << endl;
    cout << "8: " << (int*)read_customer(test, 0, 8) << endl;
    cout << (int*)read_customer(test, 0, 10) << endl;
    cout << (int*)read_customer(test, 0, 11) << endl;*/

    /*cout << "test" << endl;
    float f = 1234;
    char c[20];
    memcpy(c+4, &f, sizeof(float));
    //for (int i = 0; i < 4; i++) cout << c[i] << endl;
    float *f2 = (float*)(c+4);
     *f2 += 10;
    cout << *f2 << endl;
    float f3 = *(float*)(c+4);
    cout << f3 << endl;*/


    //cout << "int size " << sizeof(int) << endl;
    //cout << "float size " << sizeof(float) << endl;

    int insertQueryCount = 500;
    int updateQueryCount = 1000000;
    int selectQueryCount = 20000;
    int NewOrderQueryCount = 30000;

    int ifNotInitialized = InitializeOpenCL();

    //cout << "not initialized: " << ifNotInitialized << endl;

    clock_t startTime = clock();

    bool executeRowStore = true;
    bool executeColumnStore = false;
    char mode = 'G'; //C for CPU, G for GPU
    int numberOfRepetitions = 50;

    bool evaluateInsert = false;
    bool evaluateUpdate = true;
    bool evaluateSelect = false;
    bool evaluateNewOrder = false;

    bool includeTransferTime = true;

    double *timeArray = new double[numberOfRepetitions];

    ImportCSVData();

    if (!ifNotInitialized) {

        if (mode == 'G') outputFile << "Executing queries on GPU:" << endl << endl;
        else outputFile << "Executing queries on CPU:" << endl << endl;

        if (executeRowStore) {
            outputFile << "Executing Row Store" << endl;

            double timeInSeconds = 0;
            if (evaluateInsert) {
            	char* inputDataInsertRowStore = dataGenerator.GenerateRowStoreInsertData(insertQueryCount);
                for (int i = 0; i < numberOfRepetitions; i++) {
                    if (mode == 'G') {
                        timeInSeconds += queryExecutorRowStore.InsertRowStoreGPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, insertQueryCount, inputDataInsertRowStore, includeTransferTime);
                    } else {
                        timeInSeconds += queryExecutorRowStore.InsertRowStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, insertQueryCount, inputDataInsertRowStore, includeTransferTime);
                    }

                }
                delete[] inputDataInsertRowStore;
                timeInSeconds /= numberOfRepetitions;
                outputFile << "Insert Time Taken: " << timeInSeconds << endl;
                cout << "Insert Time Taken: " << timeInSeconds << endl;
            }

            if (evaluateUpdate) {
                timeInSeconds = 0;
                char* table = dataGenerator.GenerateRowStoreInsertData(10000000);
                for (int i = 0; i < numberOfRepetitions; i++) {
                    int* inputDataUpdateRowStore = dataGenerator.GenerateIndexes(updateQueryCount);

                    if (mode == 'G') {
                        timeInSeconds += queryExecutorRowStore.UpdateRowStoreGPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, updateQueryCount, inputDataUpdateRowStore, table /*rowStoreData[1]*/, includeTransferTime);
                    } else {
                        timeInSeconds += queryExecutorRowStore.UpdateRowStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, updateQueryCount, inputDataUpdateRowStore, rowStoreData[1], includeTransferTime);
                    }
                    delete[] inputDataUpdateRowStore;
                }
                timeInSeconds /= numberOfRepetitions;
                outputFile << "Update Time Taken: " << timeInSeconds << endl;
                cout << "Update Time Taken: " << timeInSeconds << endl;
            }

            if (evaluateSelect) {
                timeInSeconds = 0;
                for (int i = 0; i < numberOfRepetitions; i++) {
                    int* inputDataSelectRowStore = dataGenerator.GenerateIndexes(selectQueryCount);
                    if (mode == 'G') {
                        timeInSeconds += queryExecutorRowStore.SelectRowStoreGPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, selectQueryCount, inputDataSelectRowStore, rowStoreData[1], includeTransferTime);
                    } else {
                        timeInSeconds += queryExecutorRowStore.SelectRowStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, selectQueryCount, inputDataSelectRowStore, rowStoreData[1], includeTransferTime);
                    }
                    delete[] inputDataSelectRowStore;
                }
                timeInSeconds /= numberOfRepetitions;
                outputFile << "Select Time Taken: " << timeInSeconds << endl;
                cout << "Select Time Taken: " << timeInSeconds << endl;
            }
            
            if (evaluateNewOrder) {
                timeInSeconds = 0;
                for (int i = 0; i < numberOfRepetitions; i++) {
                    InputDataNewOrderRowStore inputDataNewOrderRowStore = dataGenerator.GenerateRowStoreNewOrderData(NewOrderQueryCount, rowStoreData);
                    if (mode == 'G') {
                        timeInSeconds += queryExecutorRowStore.NewOrderRowStoreGPU(GPUcontext, GPUdeviceIdCount,GPUdeviceIds, NewOrderQueryCount, inputDataNewOrderRowStore, includeTransferTime);
                    } else {
                        timeInSeconds += queryExecutorRowStore.NewOrderRowStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, NewOrderQueryCount, inputDataNewOrderRowStore, includeTransferTime);
                    }
                }
                timeInSeconds /= numberOfRepetitions;
                outputFile << "New Order Time Taken: " << timeInSeconds << endl;
                cout << "New Order Time Taken: " << timeInSeconds << endl;
            }


            for (int i = 0; i < 9; i++) {
                free(rowStoreData[i]);
            }
            delete[] rowStoreData;
        }

        if (executeColumnStore) {
            outputFile << "Executing Column Store" << endl;

            double timeInSeconds = 0;

            if (evaluateInsert) {
                for (int i = 0; i < numberOfRepetitions; i++) {
                    InputDataInsertColumnStore inputDataInsertColumnStore = dataGenerator.GenerateColumnStoreInsertData(insertQueryCount, columnStoreData);
                    if (mode == 'G') {
                        timeInSeconds += queryExecutorColumnStore.InsertColumnStoreGPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, insertQueryCount, inputDataInsertColumnStore, includeTransferTime);
                    } else {
                        timeInSeconds += queryExecutorColumnStore.InsertColumnStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, insertQueryCount, inputDataInsertColumnStore, includeTransferTime);
                    }
                }
                timeInSeconds /= numberOfRepetitions;
                outputFile << "Insert Time Taken: " << timeInSeconds << endl;
                cout << "Insert Time Taken: " << timeInSeconds << endl;
            }

            if (evaluateUpdate) {
                timeInSeconds = 0;
                for (int i = 0; i < numberOfRepetitions; i++) {
                    int* inputDataUpdateColumnStore = dataGenerator.GenerateIndexes(updateQueryCount);
                    if (mode == 'G') {
                        timeInSeconds += queryExecutorColumnStore.UpdateColumnStoreGPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, updateQueryCount, inputDataUpdateColumnStore, &columnStoreData, includeTransferTime);
                        //cout << "3: " << timeInSeconds << endl;
                    } else {
                        timeInSeconds += queryExecutorColumnStore.UpdateColumnStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, updateQueryCount, inputDataUpdateColumnStore, &columnStoreData, includeTransferTime);
                    }
                    delete[] inputDataUpdateColumnStore;

                }
                timeInSeconds /= numberOfRepetitions;
                outputFile << "Update Time Taken: " << timeInSeconds << endl;
                cout << "Update Time Taken: " << timeInSeconds << endl;
            }

            if (evaluateSelect) {
                timeInSeconds = 0;
                for (int i = 0; i < numberOfRepetitions; i++) {
                    int* inputDataSelectColumnStore = dataGenerator.GenerateIndexes(selectQueryCount);
                    if (mode == 'G') {
                        timeInSeconds += queryExecutorColumnStore.SelectColumnStoreGPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, selectQueryCount, inputDataSelectColumnStore, &columnStoreData, includeTransferTime);
                    } else {
                        timeInSeconds += queryExecutorColumnStore.SelectColumnStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, GPUcontext, GPUdeviceIdCount,
                                GPUdeviceIds, selectQueryCount, inputDataSelectColumnStore, &columnStoreData, includeTransferTime);
                    }
                    delete[] inputDataSelectColumnStore;
                }
                timeInSeconds /= numberOfRepetitions;
                outputFile << "Select Time Taken: " << timeInSeconds << endl;
                cout << "Select Time Taken: " << timeInSeconds << endl;
            }

            if (evaluateNewOrder) {
                timeInSeconds = 0;
                for (int i = 0; i < numberOfRepetitions; i++) {
                    InputDataNewOrderColumnStore inputDataNewOrderColumnStore = dataGenerator.GenerateColumnStoreNewOrderData(NewOrderQueryCount, columnStoreData);
                    if (mode == 'G') {
                        timeInSeconds += queryExecutorColumnStore.NewOrderColumnStoreGPU(GPUcontext, GPUdeviceIdCount, GPUdeviceIds, NewOrderQueryCount, inputDataNewOrderColumnStore, includeTransferTime);
                    } else {
                        timeInSeconds += queryExecutorColumnStore.NewOrderColumnStoreCPU(CPUcontext, CPUdeviceIdCount, CPUdeviceIds, NewOrderQueryCount, inputDataNewOrderColumnStore, includeTransferTime);
                    }
                }
                timeInSeconds /= numberOfRepetitions;
                outputFile << "New Order Time Taken: " << timeInSeconds << endl;
                cout << "New Order Time Taken: " << timeInSeconds << endl;
            }


        }

        if (mode == 'G') clReleaseContext(GPUcontext);
        else clReleaseContext(CPUcontext);

        clock_t endTime = clock();
        clock_t clockTicksTaken = endTime - startTime;
        double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;

        outputFile << "duration: " << timeInSeconds << '\n' << endl;
        outputFile.close();


    } else {
        cout << "Problem loading Platform or device check OpenCL initialization" << endl;
        //return 0;
    }

}




