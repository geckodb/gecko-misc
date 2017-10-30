
// TODO: Add OpenCL kernel code here.

 typedef struct vectorWrapper
{
	char outputVector[2];

}vectorWrapper;

__kernel void op_char2_insert(__global vectorWrapper* input, __global vectorWrapper* target )
{  
	const int g_id = get_global_id (0);
	
	for (int i = 0; i < 2; i++) { 
       target[g_id].outputVector[i] = input[g_id].outputVector[i];
    }

}



