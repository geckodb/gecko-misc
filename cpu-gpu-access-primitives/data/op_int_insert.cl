
// TODO: Add OpenCL kernel code here.

__kernel void op_int_insert(__global int* input, __global int* target )
{  
	const int g_id = get_global_id(0);
	target[g_id] = input[g_id];
}
