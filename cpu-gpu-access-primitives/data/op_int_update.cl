__kernel void op_int_update(__global int* target, __global int* pos)
{  
	const int g_id = get_global_id (0);
	int index = pos[g_id];
	target[index] += 10;
}
