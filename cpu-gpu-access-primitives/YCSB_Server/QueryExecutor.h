#ifndef QUERYEXECUTOR_H_
#define QUERYEXECUTOR_H_

class QueryExecutor {

	QueryExecutor();

public:

	int offsets[11];

	int insertRowStore(char* data, cl_context context, cl_device_id device, int batch_size, cl_kernel kernel);

	int insertColumnStore(UserCS data, cl_context context, cl_device_id device, int batch_size, cl_kernel* kernel);
//set of fields + result
	int readRowStore(char* data, int* indexes, long* fields, cl_context context, cl_device_id device, int batch_size, cl_kernel kernel);

	int readColumnStore(UserCS data, int** fields, int *values_per_column, cl_context context, cl_device_id device, int batch_size, cl_kernel* kernel);
//set of fields + values
	int updateRowStore(char* data, int* indexes, char* values, long* fields, cl_context context, cl_device_id device, int batch_size, cl_kernel kernel);

	int updateColumnStore(UserCS data, char** values, int** fields, int *values_per_column, cl_context context, cl_device_id device, int batch_size, cl_kernel* kernel);

};

#endif
