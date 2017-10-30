
__kernel void op_float_insert(__global float* input, __global float* target )
{  
	const int g_id = get_global_id (0);
	target[g_id] = input [g_id];
}