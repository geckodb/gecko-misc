#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable

void memcpy(global char *data, global char *value, int N) {
    for (int i = 0; i < N; i++) {
       data[i] = value[i];
    }
}

void write_value(global char *data, int tuple_pos, global char *field_value, int which_field, global int offsets[], global int *num_of_attributes) {

    int tuple_size = offsets[*num_of_attributes];
    global char *offset = data + tuple_pos * tuple_size;
    offset += offsets[which_field];
    
    memcpy(offset, field_value, (offsets[which_field+1] - offsets[which_field]));
    
}

global char *read_value(global char *data, int tuple_pos, 
                        int which_field, global int offsets[], global int *num_of_attributes) {
    int tuple_size = offsets[*num_of_attributes];
    global char *offset = data + tuple_pos * tuple_size;
    
    offset += offsets[which_field];
    return offset;
}



__kernel void op_insert_tuple(__global char* input_data, __global char* output_data, global int offsets[], global int *num_of_attributes)
{
    const int g_id = get_global_id(0);
    
    //int test = sizeof(offsets);
    //printf("%d ", test);
    
    //*check = 5;
    
    //if (g_id == *check - 1 ) {
       // *check = 1;
    //}
    
    for (int i = 0; i < *num_of_attributes; i++) {
       write_value(output_data, g_id, read_value(input_data, g_id, i, offsets, num_of_attributes), i, offsets, num_of_attributes);
    }	
}
