
__kernel void op_int_select(__global int* input_Data,__global int* output_Data, __global int* pos)
{
    const int g_id = get_global_id (0); 
	int index= pos[g_id];
	output_Data[g_id]=input_Data[index];	
}

