#include "QueryExecutor.h"

extern int tuple_size;
extern long table_size;

QueryExecutor::QueryExecutor() {
	offsets[0] = 0;
	for (int i = 1; i < 12; i++) {
		offsets[i] = 255 + 100*(i-1);
	}
}

int QueryExecutor::insertRowStore(char* data, cl_context context, cl_device_id device, int batch_size, cl_kernel kernel) {

	char* new_data = (char*)malloc(tuple_size * batch_size);

	cl_mem input_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, tuple_size * batch_size, data, nullptr);
	cl_mem output_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, tuple_size * batch_size, new_data, nullptr);
	cl_mem offset_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*12, offsets, nullptr);

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &offset_buffer);

	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, nullptr);

	clEnqueueWriteBuffer(queue, input_buffer, CL_TRUE, 0, tuple_size * batch_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, output_buffer, CL_TRUE, 0, tuple_size * batch_size, new_data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, offset_buffer, CL_TRUE, 0, sizeof(int)*12, offsets, 0, nullptr, nullptr);

	size_t global_work_size[] = {11, batch_size};

	clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);

	clFinish(queue);

	//optional, depends on where the data is
	clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, tuple_size * batch_size, new_data, 0, nullptr, nullptr);

	clReleaseMemObject(input_buffer);
	clReleaseMemObject(output_buffer);
	clReleaseMemObject(offset_buffer);

	return 0;
}

int QueryExecutor::insertColumnStore(UserCS data, cl_context context, cl_device_id device, int batch_size, cl_kernel* kernel) {

	UserCS new_data;

	cl_mem input_buffer[11];
	cl_mem output_buffer[11];

	input_buffer[0] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*255 * batch_size, data.KEY.data(), nullptr);
	output_buffer[0] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*255 * batch_size, new_data.KEY.data(), nullptr);

	input_buffer[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD0.data(), nullptr);
	output_buffer[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD0.data(), nullptr);

	input_buffer[2] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD1.data(), nullptr);
	output_buffer[2] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD1.data(), nullptr);

	input_buffer[3] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD2.data(), nullptr);
	output_buffer[3] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD2.data(), nullptr);

	input_buffer[4] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD3.data(), nullptr);
	output_buffer[4] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD3.data(), nullptr);

	input_buffer[5] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD4.data(), nullptr);
	output_buffer[5] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD4.data(), nullptr);

	input_buffer[6] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD5.data(), nullptr);
	output_buffer[6] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD6.data(), nullptr);

	input_buffer[7] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD6.data(), nullptr);
	output_buffer[7] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD6.data(), nullptr);

	input_buffer[8] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD7.data(), nullptr);
	output_buffer[8] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD7.data(), nullptr);

	input_buffer[9] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD8.data(), nullptr);
	output_buffer[9] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD8.data(), nullptr);

	input_buffer[10] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, data.FIELD9.data(), nullptr);
	output_buffer[10] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, new_data.FIELD9.data(), nullptr);

	for (int i = 0; i < 11; i++) {
		clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &input_buffer[i]);
		clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &output_buffer[i]);
	}

	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, nullptr);

	for (int i = 0; i < 11; i++) {
		int size = i = 0 ? sizeof(char)*255 : sizeof(char)*100;
		clEnqueueWriteBuffer(queue, input_buffer[i], CL_TRUE, 0, size * batch_size, data, 0, nullptr, nullptr);
	}

	size_t global_work_size[] = {batch_size};

	for (int i = 0; i < 11; i++) {
		clEnqueueNDRangeKernel(queue, kernel[i], 1, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);
	}

	clFinish(queue);

	//optional, depends on where the data is
	for (int i = 0; i < 11; i++) {
		int size = i = 0 ? sizeof(char)*255 : sizeof(char)*100;
		clEnqueueReadBuffer(queue, output_buffer[i], CL_TRUE, 0, size * batch_size, new_data, 0, nullptr, nullptr);
	}

	for (int i = 0; i < 11; i++) {
		clReleaseMemObject(input_buffer[i]);
		clReleaseMemObject(output_buffer[i]);
	}

	return 0;

}

int QueryExecutor::readRowStore(char* data, int* indexes, long* fields, cl_context context, cl_device_id device, int batch_size, cl_kernel kernel) {

	char* result = (char*)malloc(tuple_size * batch_size);

	cl_mem table_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, tuple_size * table_size, data, nullptr);
	cl_mem index_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * batch_size, indexes, nullptr);
	cl_mem field_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(long) * batch_size, fields, nullptr);
	cl_mem output_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, tuple_size * batch_size, result, nullptr);
	cl_mem offset_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*12, offsets, nullptr);

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &table_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &index_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &field_buffer);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), &output_buffer);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), &offset_buffer);

	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, nullptr);

	clEnqueueWriteBuffer(queue, table_buffer, CL_TRUE, 0, tuple_size * table_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, index_buffer, CL_TRUE, 0, sizeof(int) * batch_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, field_buffer, CL_TRUE, 0, sizeof(long) * batch_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, output_buffer, CL_TRUE, 0, tuple_size * batch_size, result, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, offset_buffer, CL_TRUE, 0, sizeof(int)*12, offsets, 0, nullptr, nullptr);

	size_t global_work_size[] = {11, batch_size};

	clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);

	clFinish(queue);

	clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, tuple_size * batch_size, new_data, 0, nullptr, nullptr);

	clReleaseMemObject(table_buffer);
	clReleaseMemObject(index_buffer);
	clReleaseMemObject(field_buffer);
	clReleaseMemObject(output_buffer);
	clReleaseMemObject(offset_buffer);

	return 0;

}

int QueryExecutor::readColumnStore(UserCS data, int** fields, int *values_per_column, cl_context context, cl_device_id device, int batch_size, cl_kernel* kernel) {

	UserCS result;

	cl_mem table_buffer[11];
	cl_mem index_buffer;
	cl_mem field_buffer;
	cl_mem output_buffer[11];

	table_buffer[0] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*255 * table_size, data.KEY.data(), nullptr);
	output_buffer[0] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*255 * batch_size, result.KEY.data(), nullptr);

	table_buffer[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD0.data(), nullptr);
	output_buffer[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD0.data(), nullptr);

	table_buffer[2] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD1.data(), nullptr);
	output_buffer[2] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD1.data(), nullptr);

	table_buffer[3] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD2.data(), nullptr);
	output_buffer[3] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD2.data(), nullptr);

	table_buffer[4] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD3.data(), nullptr);
	output_buffer[4] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD3.data(), nullptr);

	table_buffer[5] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD4.data(), nullptr);
	output_buffer[5] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD4.data(), nullptr);

	table_buffer[6] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD5.data(), nullptr);
	output_buffer[6] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD6.data(), nullptr);

	table_buffer[7] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD6.data(), nullptr);
	output_buffer[7] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD6.data(), nullptr);

	table_buffer[8] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD7.data(), nullptr);
	output_buffer[8] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD7.data(), nullptr);

	table_buffer[9] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD8.data(), nullptr);
	output_buffer[9] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD8.data(), nullptr);

	table_buffer[10] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD9.data(), nullptr);
	output_buffer[10] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * batch_size, result.FIELD9.data(), nullptr);

	index_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * batch_size, indexes, nullptr);
	field_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(long) * batch_size, fields, nullptr);

	for (int i = 0; i < 11; i++) {
		clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &input_buffer[i]);
		clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &index_buffer);
		clSetKernelArg(kernel[i], 2, sizeof(cl_mem), &field_buffer);
		clSetKernelArg(kernel[i], 3, sizeof(cl_mem), &output_buffer[i]);
	}

	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, nullptr);

	clEnqueueWriteBuffer(queue, table_buffer[i], CL_TRUE, 0, size * batch_size, data, 0, nullptr, nullptr);

	clEnqueueWriteBuffer(queue, index_buffer, CL_TRUE, 0, sizeof(int) * batch_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, field_buffer, CL_TRUE, 0, sizeof(long) * batch_size, data, 0, nullptr, nullptr);

	size_t global_work_size[] = {batch_size};

	for (int i = 0; i < 11; i++) {
		clEnqueueNDRangeKernel(queue, kernel[i], 1, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);
	}

	clFinish(queue);

	clEnqueueReadBuffer(queue, output_buffer[0], CL_TRUE, 0, sizeof(char)*255 * batch_size, result.KEY.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[1], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD0.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[2], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD1.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[3], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD2.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[4], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD3.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[5], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD4.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[6], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD5.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[7], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD6.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[8], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD7.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[9], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD8.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, output_buffer[10], CL_TRUE, 0, sizeof(char)*100 * batch_size, result.FIELD9.data(), 0, nullptr, nullptr);


	for (int i = 0; i < 11; i++) {
		clReleaseMemObject(table_buffer[i]);
		clReleaseMemObject(output_buffer[i]);
	}
	clReleaseMemObject(index_buffer);
	clReleaseMemObject(field_buffer);

	return 0;

}

int QueryExecutor::updateRowStore(char* data, int* indexes, char* values, long* fields, cl_context context, cl_device_id device, int batch_size, cl_kernel kernel) {

	cl_mem table_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, tuple_size * table_size, data, nullptr);
	cl_mem index_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * batch_size, indexes, nullptr);
	cl_mem field_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(long) * batch_size, fields, nullptr);
	cl_mem value_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * total_fields * batch_size, fields, nullptr);
	cl_mem offset_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int)*12, offsets, nullptr);

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &table_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &index_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &field_buffer);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), &value_buffer);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), &offset_buffer);

	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, nullptr);

	clEnqueueWriteBuffer(queue, table_buffer, CL_TRUE, 0, tuple_size * table_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, index_buffer, CL_TRUE, 0, sizeof(int) * batch_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, field_buffer, CL_TRUE, 0, sizeof(long) * batch_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, offset_buffer, CL_TRUE, 0, sizeof(int)*12, offsets, 0, nullptr, nullptr);

	size_t global_work_size[] = {11, batch_size};

	clEnqueueNDRangeKernel(queue, kernel, 2, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);

	clFinish(queue);

	clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, tuple_size * batch_size, new_data, 0, nullptr, nullptr);

	clReleaseMemObject(table_buffer);
	clReleaseMemObject(index_buffer);
	clReleaseMemObject(field_buffer);
	clReleaseMemObject(offset_buffer);

	return 0;

}

int QueryExecutor::updateColumnStore(UserCS data, char** values, int** fields, int *values_per_column, cl_context context, cl_device_id device, int batch_size, cl_kernel* kernel) {

	cl_mem table_buffer[11];
	cl_mem index_buffer;
	cl_mem field_buffer;
	cl_mem value_buffer;

	table_buffer[0] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*255 * table_size, data.KEY.data(), nullptr);
	table_buffer[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD0.data(), nullptr);
	table_buffer[2] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD1.data(), nullptr);
	table_buffer[3] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD2.data(), nullptr);
	table_buffer[4] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD3.data(), nullptr);
	table_buffer[5] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD4.data(), nullptr);
	table_buffer[6] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD5.data(), nullptr);
	table_buffer[7] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD6.data(), nullptr);
	table_buffer[8] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD7.data(), nullptr);
	table_buffer[9] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD8.data(), nullptr);
	table_buffer[10] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * table_size, data.FIELD9.data(), nullptr);

	index_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * batch_size, indexes, nullptr);
	field_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(long) * batch_size, fields, nullptr);
	value_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(char)*100 * total_fields * batch_size, fields, nullptr);

	for (int i = 0; i < 11; i++) {
		clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &input_buffer[i]);
		clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &index_buffer);
		clSetKernelArg(kernel[i], 2, sizeof(cl_mem), &field_buffer);
		clSetKernelArg(kernel[i], 3, sizeof(cl_mem), &value_buffer);
		clSetKernelArg(kernel[i], 4, sizeof(cl_mem), &output_buffer[i]);
	}

	cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, nullptr);

	for (int i = 0; i < 11; i++) {
		int size = i = 0 ? sizeof(char)*255 : sizeof(char)*100;
		clEnqueueWriteBuffer(queue, table_buffer[i], CL_TRUE, 0, size * batch_size, data, 0, nullptr, nullptr);
	}

	clEnqueueWriteBuffer(queue, index_buffer, CL_TRUE, 0, sizeof(int) * batch_size, data, 0, nullptr, nullptr);
	clEnqueueWriteBuffer(queue, field_buffer, CL_TRUE, 0, sizeof(long) * batch_size, data, 0, nullptr, nullptr);

	size_t global_work_size[] = {batch_size};

	for (int i = 0; i < 11; i++) {
		clEnqueueNDRangeKernel(queue, kernel[i], 1, nullptr, global_work_size, nullptr, 0, nullptr, nullptr);
	}

	clFinish(queue);

	//optional, depends on where the data is
	clEnqueueReadBuffer(queue, table_buffer[0], CL_TRUE, 0, sizeof(char)*255 * batch_size, data.KEY.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[1], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD0.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[2], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD1.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[3], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD2.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[4], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD3.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[5], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD4.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[6], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD5.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[7], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD6.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[8], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD7.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[9], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD8.data(), 0, nullptr, nullptr);
	clEnqueueReadBuffer(queue, table_buffer[10], CL_TRUE, 0, sizeof(char)*100 * batch_size, data.FIELD9.data(), 0, nullptr, nullptr);

	for (int i = 0; i < 11; i++) {
		clReleaseMemObject(table_buffer[i]);
	}
	clReleaseMemObject(index_buffer);
	clReleaseMemObject(field_buffer);

	return 0;

}
