global void memcpy(global char *data, global char *value, int N) {
    for (int i = 0; i < N; i++) {
       data[i] = value[i];
    }
}

global void write_value(global char *data, int tuple_pos, global char *field_value, 
                        int which_field, global int offsets[]) {

    int tuple_size = offsets[11];
    global char *offset = data + tuple_pos * tuple_size;
    offset += offsets[which_field];
    
    memcpy(offset, field_value, (offsets[which_field+1] - offsets[which_field]));
    
}

global char *read_value(global char *data, int tuple_pos, 
                        int which_field, global int offsets[]) {
    int tuple_size = offsets[11];
    global char *offset = data + tuple_pos * tuple_size;
    
    offset += offsets[which_field];
    return offset;
}

kernel void insert_value_rs(global char* input_data, global char* output_data, global int* offsets)
{
    const int g_id = get_global_id(1);
    const int attr_id = get_global_id(0);
    
    write_value(output_data, g_id, read_value(input_data, g_id, attr_id, offsets), attr_id, offsets);
}
