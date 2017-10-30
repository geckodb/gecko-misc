
 __kernel void op_float_update(__global float* input_data, __global int* pos)
{
    const int g_id = get_global_id(0);
	int index = pos[g_id];
	input_data[index] += 10;
}

