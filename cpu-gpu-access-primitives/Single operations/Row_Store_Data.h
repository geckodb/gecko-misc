#ifndef ROWSTOREDATA_H
#define ROWSTOREDATA_H
#include<string>

using namespace std;

/*struct District
{
	short int d_id;
	short int d_w_id;
	float d_ytd;
	int d_next_o_id;
	float d_tax;
	char d_name[10];
	char d_street_1[20];
	char d_street_2[20];
	char d_city[20];
	char d_state[2];
	char d_zip[9];
}; */

size_t districtSize();


void update_district(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_district(char *raw_data, size_t tuple_pos, size_t which_field);

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
	float c_ytd_payment;
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

size_t customerSize();

void update_customer(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_customer(char *raw_data, size_t tuple_pos, size_t which_field);

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

size_t historySize();

void update_history(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

/*struct Item
{
	int i_id;
	char i_name[30];
	float i_price;
	char i_data[50];
	int i_im_id;
};*/

size_t itemSize();

void update_item(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_item(char *raw_data, size_t tuple_pos, size_t which_field);

/*struct New_Order
{
	int no_o_id;
	short int no_d_id;
	int no_w_id;
};*/

size_t newOrderSize();

void update_new_order(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_new_order(char *raw_data, size_t tuple_pos, size_t which_field);

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

size_t orderLineSize();

void update_order_line(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_order_line(char *raw_data, size_t tuple_pos, size_t which_field);

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

size_t ordersSize();

void update_orders(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_orders(char *raw_data, size_t tuple_pos, size_t which_field);

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

size_t stockSize();

void update_stock(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_stock(char *raw_data, size_t tuple_pos, size_t which_field);

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

size_t warehouseSize();

void update_warehouse(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field);

char *read_warehouse(char *raw_data, size_t tuple_pos, size_t which_field);

/*struct NewOrderQueryDataRowStore
{
	short int w_id;
	short int d_id;
	int c_id;
	int	o_id;
	short int ol_number;
	short int o_carrier_id;
	short int ol_cnt;
	char entry_d[10];
	char ol_dist_info[20];
	int	item_id[5];
	float item_price[5];
	float   w_tax;
	float	c_discount;
	float	d_tax;
};*/

/*void update_tmp(char *raw_data, size_t tuple_pos, char *field_value, size_t which_field) {

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
} */

/*struct CustomerCompare
{
	int id;
	string first_name;
};*/

/*class LoadedData
{
    LoadedData() {
        this->orders_Data = (char *) malloc(1024);
        this->no_Data = (char *) malloc(1024);
        this->ol_Data = (char *) malloc(1024);
        this->item_Data = (char *) malloc(1024);
        this->cust_Data = (char *) malloc(1024);
        this->dist_Data = (char *) malloc(1024);
        this->wh_Data = (char *) malloc(1024);
        this->stock_Data = (char *) malloc(1024);
        this->hist_Data = (char *) malloc(1024);
        this->customers = (char *) malloc(1024);
    }
public:
	 char* orders_Data;
         unsigned int orders_Data_size;
	 char* no_Data;
         unsigned int no_Data_size;
	 char* ol_Data;
         unsigned int ol_Data_size;
	 char* item_Data;
         unsigned int item_Data_size;
	 char* cust_Data;
         unsigned int cust_Data_size;
	 char* dist_Data;
         unsigned int dist_Data_size;
	 char* wh_Data;
         unsigned int wh_Data_size;
	 char* stock_Data;
         unsigned int stock_Data_size;
	 char* hist_Data;
         unsigned int hist_Data_size;
	 char* customers[100]; //why [100]?
         unsigned int customers_Data_size;
}; */

struct InputDataNewOrderRowStore {
    char* NewOrder;
    char* OrderLine;
    char* Orders;
    int total_order_lines;
};

struct vectorWrapper
{
	char outputVector[30];

}; 

#endif