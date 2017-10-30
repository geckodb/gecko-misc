 /*struct Customer
{
	int c_id;
	short int c_d_id;
	short int c_w_id;
	float c_discount;
	float c_credit_lim;
	char c_last[20];
	char c_first[20];
	char c_credit[2];
	float c_balance;
	float c_ytd_payment; //9
	short int c_payment_cnt;
	short int c_delivery_cnt;
	char c_street_1[20];
	char c_street_2[20];
	char c_city[20];
	char c_state[2];
	char c_zip[9];
	char c_phone[20];
	char c_since[10];
	char c_middle[2];
	char c_data[30];
};*/

void memcpy(global char *data, global char *value, int N) {
    for (int i = 0; i < N; i++) {
       data[i] = value[i];
    }
}

void write_value(global char *data, int tuple_pos, global char *field_value, 
                        int which_field, global int offsets[], global int *num_of_attributes) {

    int tuple_size = offsets[*num_of_attributes];
    global char *offset = data + tuple_pos * tuple_size;
    offset += offsets[which_field];
    
    memcpy(offset, field_value, (offsets[which_field+1] - offsets[which_field]));
    
}

global char *read_value(global char *data, int tuple_pos, 
                        int which_field, global int offsets[], global int *num_of_attributes) {
    int tuple_size = offsets[*num_of_attributes+1];
    global char *offset = data + tuple_pos * tuple_size;
    offset += offsets[which_field];
    return offset;
}


kernel void op_update_Customers(global char* input_data, global int* pos, global int offsets[], 
                                global int *num_of_attributes, global char* types)
{
    int g_id = get_global_id(0);
    int index = pos[g_id];

    for (int j = 0; j < *num_of_attributes+1; j++) {
    if (types[j] == 'f') { // if float
        
        global float *p;
        p = (global float*)read_value(input_data, index, j, offsets, num_of_attributes);
        *p += 10;
    } 
    else if (types[j] == 'i') { // if int
        global int *p;
        p = (global int*)read_value(input_data, index, j, offsets, num_of_attributes);
        *p += 10;
    }
    else { // if char
        write_value(input_data, index, read_value(input_data, index, j, offsets, num_of_attributes), j, offsets, num_of_attributes);
    }
    }

}

