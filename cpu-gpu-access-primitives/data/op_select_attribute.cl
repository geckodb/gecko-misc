 /*struct Customer
{
	int c_id;
	short int c_d_id;
	short int c_w_id;
	float c_discount;
	float c_credit_lim;
	char c_last[20];
	char c_first[20];
	char c_credit[2]; //7
	float c_balance;
	float c_ytd_payment;
	short int c_payment_cnt;
	short int c_delivery_cnt;
	char c_street_1[20]; //12
	char c_street_2[20];
	char c_city[20];
	char c_state[2];
	char c_zip[9];
	char c_phone[20];
	char c_since[10];
	char c_middle[2];
	char c_data[30];
};*/

global void memcpy(global char *raw_data, global char *value, size_t N) {
    for (size_t i = 0; i < N; i++) {
       raw_data[i] = value[i];
    }
}

global void update_customer(global char *raw_data, size_t tuple_pos, global char *field_value, size_t which_field, global int offsets[]) {

    size_t tuple_size = offsets[21];
    global char *offset = raw_data + tuple_pos * tuple_size;
    offset += offsets[which_field];
    
    memcpy(offset, field_value, (offsets[which_field+1] - offsets[which_field]));
    
}

global char *read_customer(global char *raw_data, size_t tuple_pos, size_t which_field, global int offsets[]) {
    size_t tuple_size = offsets[21];
    global char *offset = raw_data + tuple_pos * tuple_size;
    
    offset += offsets[which_field];
    return offset;
}



__kernel void op_select_attribute(__global char* input_data, __global char* output_data, __global int* pos, global int offsets[], int p)
{
    const int g_id = get_global_id(0);
    int index = pos[g_id];

    update_customer(output_data, g_id, read_customer(input_data, index, p, offsets), p, offsets);
	
}
