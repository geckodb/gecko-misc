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

kernel void update_single_value(global char* input_data, global int* indexes, global long* fields,
								global char* values, global int offsets[])
{
    const int g_id = get_global_id(1);
    const int attr_id = get_global_id(0);

	write_value(input_data, index, read_value(input_data, index, attr_id, offsets), attr_id, offsets);
}
