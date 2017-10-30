
typedef struct charArray
{
	char field[100];

} charArray;


kernel void read_value_cs(global charArray* input, global charArray* output, global int* rows)
{
    const int g_id = get_global_id(0); 
	int index = rows[g_id];

	for (int i = 0; i < 100; i++) { 
       output[g_id].field[i] = input[index].field[i];
    }
}



