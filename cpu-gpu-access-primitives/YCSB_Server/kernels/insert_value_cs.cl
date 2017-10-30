
// TODO: Add OpenCL kernel code here.

 typedef struct vectorWrapper
{
	char outputVector[30];

}vectorWrapper;

__kernel void op_char30_insert(__global vectorWrapper* input, __global vectorWrapper* target )
{  
	const int g_id = get_global_id (0);
	
	for (int i = 0; i < 30; i++) { 
       target[g_id].outputVector[i] = input[g_id].outputVector[i];
    }
}



