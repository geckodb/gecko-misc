
// TODO: Add OpenCL kernel code here.

 typedef struct structArray
{
	char outputVector[20];

}structArray;

 __kernel void op_char20_update(__global structArray* input_data, __global int* pos)
{

   	const int g_id = get_global_id(0);
	int index = pos[g_id];
	
	for (int i = 0; i < 20; i++) { 
       input_data[index].outputVector[i] = input_data[index].outputVector[i];
    }
}

