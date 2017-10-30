#include <string>
#include <cstring>
#include "Row_Store_Data.h"

using namespace std;

int ordersDataSize;
int olCount;
int customerDataSize;
int itemDataSize;
int districtDataSize;
int newOrderDataSize;
int orderLineDataSize;
int historyDataSize;
int stockDataSize;
int warehouseDataSize;

size_t districtSize() {
    size_t tuple_size = sizeof(short int) + sizeof(short int) + sizeof(float) + sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*3 + 2 * sizeof(char) + 9 * sizeof(char);
    return tuple_size;
}

void update_district(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(short int) + sizeof(short int) + sizeof(float) + sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*3 + 2 * sizeof(char) + 9 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 1:
            offset += sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 2:
            offset += sizeof(short int)*2;
            memcpy(offset, field_value, sizeof(float));
            break;
        case 3:
            offset += sizeof(short int)*2 + sizeof(float);
            memcpy(offset, field_value, sizeof(int));
            break;
        case 4:
            offset += sizeof(short int)*2 + sizeof(float) + sizeof(int);
            memcpy(offset, field_value, sizeof(float));
            break;
        case 5:
            offset += sizeof(short int)*2 + sizeof(float) + sizeof(int) + sizeof(float);
            memcpy(offset, field_value, sizeof(char)*10);
            break;
        case 6:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*20);
            break;
        case 7:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*20);
            break;
        case 8:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*2;
            memcpy(offset, field_value, sizeof(char)*20);
            break;
        case 9:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*3;
            memcpy(offset, field_value, sizeof(char)*2);
            break;
        case 10:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*3 + 2 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*9);
            break;
        default:
            break;
    }
}

char *read_district(char *raw_data, size_t tuple_pos, size_t which_field) {
    size_t tuple_size = sizeof(short int) + sizeof(short int) + sizeof(float) + sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*3 + 2 * sizeof(char) + 9 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            return offset;
        case 1:
            offset += sizeof(short int);
            return offset;
        case 2:
            offset += sizeof(short int)*2;
            return offset;
        case 3:
            offset += sizeof(short int)*2 + sizeof(float);
            return offset;
        case 4:
            offset += sizeof(short int)*2 + sizeof(float) + sizeof(int);
            return offset;
        case 5:
            offset += sizeof(short int)*2 + sizeof(float) + sizeof(int) + sizeof(float);
            return offset;
        case 6:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char);
            return offset;
        case 7:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char);
            return offset;
        case 8:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*2;
            return offset;
        case 9:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*3;
            return offset;
        case 10:
            offset += sizeof(short int)*2 + sizeof(float)+ sizeof(int) + sizeof(float) + 10 * sizeof(char)
             + 20 * sizeof(char)*3 + 2 * sizeof(char);
            return offset;
        default:
            return offset;
    }
}


 /*struct Customer
{
	int c_id;
	int c_d_id;
	int c_w_id;
	int c_payment_cnt;
	int c_delivery_cnt;
	float c_balance;
	float c_ytd_payment;
	float c_discount;
	float c_credit_lim;
	char c_last[20];
	char c_first[20];
	char c_credit[2];
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



size_t customerSize() {
	size_t tuple_size = sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
	+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
	+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char)
	+ 10 * sizeof(char) + 2 * sizeof(char) + 30 * sizeof(char);
	tuple_size += 4 - tuple_size%4;
	return tuple_size;
}

void update_customer(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

	size_t tuple_size = customerSize();
	char *offset = raw_data + tuple_pos * tuple_size;

	switch(which_field) {
	case 0:
		memcpy(offset, field_value, sizeof(int));
		break;
	case 1:
		offset += sizeof(int);
		memcpy(offset, field_value, sizeof(int));
		break;
	case 2:
		offset += sizeof(int)*2;
		memcpy(offset, field_value, sizeof(int));
		break;
	case 3:
		offset += sizeof(int)*3;
		memcpy(offset, field_value, sizeof(int));
		break;
	case 4:
		offset += sizeof(int)*4;
		memcpy(offset, field_value, sizeof(int));
		break;
	case 5:
		offset += sizeof(int)*5;
		memcpy(offset, field_value, sizeof(float));
		break;
	case 6:
		offset += sizeof(int)*5 + sizeof(float);
		memcpy(offset, field_value, sizeof(float));
		break;
	case 7:
		offset += sizeof(int)*5 + sizeof(float)*2;
		memcpy(offset, field_value, sizeof(float));
		break;
	case 8:
		offset += sizeof(int)*5 + sizeof(float)*3;
		memcpy(offset, field_value, sizeof(float));
		break;
	case 9:
		offset += sizeof(int)*5 + sizeof(float)*4;
		memcpy(offset, field_value, sizeof(char)*20);
		break;
	case 10:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*20);
		break;
	case 11:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*2);
		break;
	case 12:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*20);
		break;
	case 13:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*20);
		break;
	case 14:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*20);
		break;
	case 15:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*2);
		break;
	case 16:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*9);
		break;
	case 17:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*20);
		break;
	case 18:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*10);
		break;
	case 19:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char) + 10 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*2);
		break;
	case 20:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char) + 10 * sizeof(char) + 2 * sizeof(char);
		memcpy(offset, field_value, sizeof(char)*30);
		break;
	default:
		break;
	}
}

char *read_customer(char *raw_data, size_t tuple_pos, size_t which_field) {
	size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(float) + sizeof(float) + 20 * sizeof(char) + 20 * sizeof(char)
	+ 2 * sizeof(char) + sizeof(float) + sizeof(float) + sizeof(short int) + sizeof(short int) + 20 * sizeof(char) + 20 * sizeof(char)
	+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char) + 10 * sizeof(char) + 2 * sizeof(char) + 30 * sizeof(char);
	char *offset = raw_data + tuple_pos * tuple_size;

	switch(which_field) {
	case 0:
		return offset;
	case 1:
		offset += sizeof(int);
		return offset;
	case 2:
		offset += sizeof(int)*2;
		return offset;
	case 3:
		offset += sizeof(int)*3;
		return offset;
	case 4:
		offset += sizeof(int)*4;
		return offset;
	case 5:
		offset += sizeof(int)*5;
		return offset;
	case 6:
		offset += sizeof(int)*5 + sizeof(float);
		return offset;
	case 7:
		offset += sizeof(int)*5 + sizeof(float)*2;
		return offset;
	case 8:
		offset += sizeof(int)*5 + sizeof(float)*3;
		return offset;
	case 9:
		offset += sizeof(int)*5 + sizeof(float)*4;
		return offset;
	case 10:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char);
		return offset;
	case 11:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char);
		return offset;
	case 12:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char);
		return offset;
	case 13:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char);
		return offset;
	case 14:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char);
		return offset;
	case 15:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char);
		return offset;
	case 16:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char);
		return offset;
	case 17:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char);
		return offset;
	case 18:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char);
		return offset;
	case 19:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char) + 10 * sizeof(char);
		return offset;
	case 20:
		offset += sizeof(int)*5 + sizeof(float)*4 + 20 * sizeof(char) + 20 * sizeof(char)
		+ 2 * sizeof(char) + 20 * sizeof(char) + 20 * sizeof(char)
		+ 20 * sizeof(char) + 2 * sizeof(char)+ 9 * sizeof(char) + 20 * sizeof(char) + 10 * sizeof(char) + 2 * sizeof(char);
		return offset;
	default:
		return offset;
	}
}

/*struct History
{
	int h_c_id;
	short int h_c_d_id;
	short int h_c_w_id;
	short int h_d_id;
	short int h_w_id;
	char h_date[10];
	float h_amount;
	char h_data[30];
};*/

size_t historySize() {
    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + 10 * sizeof(char)
             +sizeof(float) + 30 * sizeof(char);
    return tuple_size;
}

void update_history(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + 10 * sizeof(char)
             +sizeof(float) + 30 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(int));
            break;
        case 1:
            offset += sizeof(int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 2:
            offset += sizeof(int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 3:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 4:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 5:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(char)*10);
            break;
        case 6:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + 10 * sizeof(char);
            memcpy(offset, field_value, sizeof(float));
            break;
        case 7:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + 10 * sizeof(char)
             +sizeof(float);
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        default:
            break;
    }
}

/*struct Item
{
	int i_id;
	char i_name[30];
	float i_price;
	char i_data[50];
	int i_im_id;
};*/

size_t itemSize() {
    size_t tuple_size = sizeof(int) + 30 * sizeof(char) + sizeof(float) + 50 * sizeof(char) + sizeof(int);
    return tuple_size;
}

void update_item(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(int) + 30 * sizeof(char) + sizeof(float) + 50 * sizeof(char) + sizeof(int);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(int));
            break;
        case 1:
            offset += sizeof(int);
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 2:
            offset += sizeof(int) + 30 * sizeof(char);
            memcpy(offset, field_value, sizeof(float));
            break;
        case 3:
            offset += sizeof(int) + 30 * sizeof(char) + sizeof(float);
            memcpy(offset, field_value, sizeof(char)*50);
            break;
        case 4:
            offset += sizeof(int) + 30 * sizeof(char) + sizeof(float) + 50 * sizeof(char);
            memcpy(offset, field_value, sizeof(int));
            break;
        default:
            break;
    }
}

char *read_item(char *raw_data, size_t tuple_pos, size_t which_field) {
    size_t tuple_size = sizeof(int) + 30 * sizeof(char) + sizeof(float) + 50 * sizeof(char) + sizeof(int);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            return offset;
        case 1:
            offset += sizeof(int);
            return offset;
        case 2:
            offset += sizeof(int) + 30 * sizeof(char);
            return offset;
        case 3:
            offset += sizeof(int) + 30 * sizeof(char) + sizeof(float);
            return offset;
        case 4:
            offset += sizeof(int) + 30 * sizeof(char) + sizeof(float) + 50 * sizeof(char);
            return offset;
        default:
            break;
    }
    
}

/*struct New_Order
{
	int no_o_id;
	short int no_d_id;
	int no_w_id;
};*/

size_t newOrderSize() {
    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(int);
    return tuple_size;
}

void update_new_order(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(int);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(int));
            break;
        case 1:
            offset += sizeof(int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 2:
            offset += sizeof(int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(int));
            break;
        default:
            break;
    }
}

char *read_new_order(char *raw_data, size_t tuple_pos, size_t which_field) {
    
    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(int);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            return offset;
        case 1:
            offset += sizeof(int);
            return offset;
        case 2:
            offset += sizeof(int) + sizeof(short int);
            return offset;
        default:
            return offset;
    }
    
}

/*struct Order_Line
{
	int ol_o_id;
	short int ol_d_id;
	short int ol_w_id;
	short int ol_number;
	int ol_i_id;
	char ol_delivery_d[20];
	float ol_amount;
	short int ol_supply_w_id;
	short int ol_quantity;
	char ol_dist_info[30];
};*/

size_t orderLineSize() {
    size_t tuple_size = sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float) + sizeof(short int) + sizeof(short int) + 30 * sizeof(char);
    return tuple_size;
}

void update_order_line(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float) + sizeof(short int) + sizeof(short int) + 30 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 1:
            offset += sizeof(int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 2:
            offset += sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 3:
            offset += sizeof(short int)*2;
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 4:
            offset += sizeof(short int)*3;
            memcpy(offset, field_value, sizeof(int));
            break;
        case 5:
            offset += sizeof(short int)*4 + sizeof(int);
            memcpy(offset, field_value, sizeof(char)*20);
            break;
        case 6:
            offset += sizeof(short int)*4 + sizeof(int) + 20 * sizeof(char);
            memcpy(offset, field_value, sizeof(float));
            break;
        case 7:
            offset += sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 8:
            offset += sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 9:
            offset += sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        default:
            break;
    }
}

char *read_order_line(char *raw_data, size_t tuple_pos, size_t which_field) {
    
    size_t tuple_size = sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float) + sizeof(short int) + sizeof(short int) + 30 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            return offset;
        case 1:
            offset += sizeof(int);
            return offset;
        case 2:
            offset += sizeof(short int);
            return offset;
        case 3:
            offset += sizeof(short int)*2;
            return offset;
        case 4:
            offset += sizeof(short int)*3;
            return offset;
        case 5:
            offset += sizeof(short int)*4 + sizeof(int);
            return offset;
        case 6:
            offset += sizeof(short int)*4 + sizeof(int) + 20 * sizeof(char);
            return offset;
        case 7:
            offset += sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float);
            return offset;
        case 8:
            offset += sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float) + sizeof(short int);
            return offset;
        case 9:
            offset += sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(short int) + sizeof(int) + 20 * sizeof(char)
             + sizeof(float) + sizeof(short int) + sizeof(short int);
            return offset;
        default:
            return offset;
    }
    
}

/*struct Orders
{
	int o_id;
	short int o_d_id;
	short int o_w_id;
	int o_c_id;
	short int o_carrier_id;
	short int o_ol_cnt;
	short int o_all_local;
	char o_entry_d[30];
};*/

size_t ordersSize() {
    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) 
            + 30 * sizeof(char);
    return tuple_size;
}

void update_orders(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) 
            + 30 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(int));
            break;
        case 1:
            offset += sizeof(int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 2:
            offset += sizeof(int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 3:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(int));
            break;
        case 4:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 5:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) 
            + 30 * sizeof(char);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 6:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 7:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        default:
            break;
    }
}

char *read_orders(char *raw_data, size_t tuple_pos, size_t which_field) {
    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) 
            + 30 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            return offset;
        case 1:
            offset += sizeof(int);
            return offset;
        case 2:
            offset += sizeof(int) + sizeof(short int);
            return offset;
        case 3:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int);
            return offset;
        case 4:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int);
            return offset;
        case 5:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int) 
            + 30 * sizeof(char);
            return offset;
        case 6:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int);
            return offset;
        case 7:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(short int);
            return offset;
        default:
            return offset;
    }
}

/*struct Stock
{
	int s_i_id;
	short int s_w_id;
	short int s_quantity;
	int s_ytd;
	short int s_order_cnt;
	short int s_remote_cnt;
	char s_data[40];
	char s_dist_01[30];
	char s_dist_02[30];
	char s_dist_03[30];
	char s_dist_04[30];
	char s_dist_05[30];
	char s_dist_06[30];
	char s_dist_07[30];
	char s_dist_08[30];
	char s_dist_09[30];
	char s_dist_10[30];
};*/

size_t stockSize() {
    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*10;
    return tuple_size;
}

void update_stock(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*10;
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(int));
            break;
        case 1:
            offset += sizeof(int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 2:
            offset += sizeof(int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 3:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(int));
            break;
        case 4:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 5:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 6:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int);
            memcpy(offset, field_value, sizeof(char)*40);
            break;
        case 7:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 8:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 9:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*2;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 10:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*3;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 11:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*4;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 12:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*5;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 13:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*6;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 14:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*7;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 15:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*8;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        case 16:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*9;
            memcpy(offset, field_value, sizeof(char)*30);
            break;
        default:
            break;
    }
}

char *read_stock(char *raw_data, size_t tuple_pos, size_t which_field) {
    size_t tuple_size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*10;
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            return offset;
        case 1:
            offset += sizeof(int);
            return offset;
        case 2:
            offset += sizeof(int) + sizeof(short int);
            return offset;
        case 3:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int);
            return offset;
        case 4:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int);
            return offset;
        case 5:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int);
            return offset;
        case 6:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int);
            return offset;
        case 7:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char);
            return offset;
        case 8:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char);
            return offset;
        case 9:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*2;
            return offset;
        case 10:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*3;
            return offset;
        case 11:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*4;
            return offset;
        case 12:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*5;
            return offset;
        case 13:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*6;
            return offset;
        case 14:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*7;
            return offset;
        case 15:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*8;
            return offset;
        case 16:
            offset += sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + sizeof(short int)
            + 40 * sizeof(char) + 30 * sizeof(char)*9;
            return offset;
        default:
            break;
    }
    
}

/*struct Warehouse
{
	short int W_ID;
	float w_ytd;
	float w_tax;
	char w_name[10];
	char w_street_1[20];
	char w_street_2[20];
	char w_city[20];
	char w_state[2];
	char w_zip[9];
};*/

size_t warehouseSize() {
    size_t tuple_size = sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*3 + 2 * sizeof(char) + 9 * sizeof(char);
    return tuple_size;
}

void update_warehouse(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

    size_t tuple_size = sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*3 + 2 * sizeof(char) + 9 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            memcpy(offset, field_value, sizeof(short int));
            break;
        case 1:
            offset += sizeof(short int);
            memcpy(offset, field_value, sizeof(float));
            break;
        case 2:
            offset += sizeof(short int) + sizeof(float);
            memcpy(offset, field_value, sizeof(float));
            break;
        case 3:
            offset += sizeof(short int) + sizeof(float) + sizeof(float);
            memcpy(offset, field_value, sizeof(char)*10);
            break;
        case 4:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*20);
            break;
        case 5:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*20);
            break;
        case 6:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*2;
            memcpy(offset, field_value, sizeof(char)*20);
            break;
        case 7:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*3;
            memcpy(offset, field_value, sizeof(char)*2);
            break;
        case 8:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*3 + 2 * sizeof(char);
            memcpy(offset, field_value, sizeof(char)*9);
            break;
        default:
            break;
    }
}

char *read_warehouse(char *raw_data, size_t tuple_pos, size_t which_field) {
    
    size_t tuple_size = sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*3 + 2 * sizeof(char) + 9 * sizeof(char);
    char *offset = raw_data + tuple_pos * tuple_size;
    
    switch(which_field) {
        case 0:
            return offset;
        case 1:
            offset += sizeof(short int);
            return offset;
        case 2:
            offset += sizeof(short int) + sizeof(float);
            return offset;
        case 3:
            offset += sizeof(short int) + sizeof(float) + sizeof(float);
            return offset;
        case 4:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char);
            return offset;
        case 5:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char);
            return offset;
        case 6:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*2;
            return offset;
        case 7:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*3;
            return offset;
        case 8:
            offset += sizeof(short int) + sizeof(float) + sizeof(float) + 10 * sizeof(char) + 20 * sizeof(char)*3 + 2 * sizeof(char);
            return offset;
        default:
            return offset;
    }
}


