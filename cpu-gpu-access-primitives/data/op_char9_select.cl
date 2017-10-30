
 typedef struct structArray
{
	char outputVector[9];

}structArray;

__kernel void op_char9_select(__global structArray* input_Data,__global structArray* output_Data, __global int* pos)
{
    const int g_id = get_global_id (0); 
	int index= pos[g_id];
	for (int i = 0; i < 9; i++) { 
       output_Data[g_id].outputVector[i] = input_Data[index].outputVector[i];
    }
}



