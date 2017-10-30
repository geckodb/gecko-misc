

#include "QueryDataGenerator.h"
#include <time.h>
#include <algorithm>
#include <random>

extern int ordersDataSize;
extern int customerDataSize;

QueryDataGenerator::QueryDataGenerator() {
}

int QueryDataGenerator::GenerateRandomNumber(int min, int max) {
    int randNum = rand() % (max - min + 1) + min;
    //std::mt19937 rng(1);
    //std::uniform_int_distribution<int> gen(min, max);
    //int randNum = gen(rng);
    return randNum;
}

string QueryDataGenerator::GenerateRandomString(int length) {
	static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string result;
	result.resize(length);

	srand(time(NULL));
	for (int i = 0; i < length-1; i++)
		result[i] = charset[rand() % charset.length()];

        result[length-1] = '\0';
	return result;
}

string QueryDataGenerator::CurrentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof (buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

QueryDataGenerator::~QueryDataGenerator() {
}

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

char* QueryDataGenerator::GenerateRowStoreInsertData(int numberOfQueries) {
    
    char* inputData = (char*) malloc(customerSize() * numberOfQueries);
    short int w_id = 1;
    string c = GenerateRandomString(30);
    char chr[30];
    strcpy(chr, c.c_str());
    for (int i = 0; i < numberOfQueries; i++) {
        int c_id = customerDataSize + 1 + i;
        update_customer(inputData, i, (char*)&c_id , 0);
        short int d_id = (i + 1) % 10;
        update_customer(inputData, i, (char*)&d_id , 1);
        update_customer(inputData, i, (char*)&w_id , 2);
        float f = GenerateRandomNumber(1, 100);
        update_customer(inputData, i, (char*)&f , 3);
        update_customer(inputData, i, (char*)&f , 4);
        update_customer(inputData, i, (char*)chr, 5);
        update_customer(inputData, i, (char*)chr, 6);
        update_customer(inputData, i, (char*)chr, 7);
        update_customer(inputData, i, (char*)&f , 9);
        short int s = GenerateRandomNumber(1, 100);
        update_customer(inputData, i, (char*)&s , 10);
        update_customer(inputData, i, (char*)&s , 11);
        update_customer(inputData, i, (char*)chr, 12);
        update_customer(inputData, i, (char*)chr, 13);
        update_customer(inputData, i, (char*)chr, 14);
        update_customer(inputData, i, (char*)chr, 15);
        update_customer(inputData, i, (char*)chr, 16);
        update_customer(inputData, i, (char*)chr, 17);
        update_customer(inputData, i, (char*)chr, 18);
        update_customer(inputData, i, (char*)chr, 19);
        update_customer(inputData, i, (char*)chr, 20);
    }

    return inputData;
}

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

InputDataInsertColumnStore QueryDataGenerator::GenerateColumnStoreInsertData(int numberOfQueries, ColumnStoreData columnStoreData) {
    InputDataInsertColumnStore inputData;
    
    charArray2 c2;
    charArray9 c9;
    charArray10 c10;
    charArray20 c20;
    charArray30 c30;

    strcpy(c2.array, GenerateRandomString(2).c_str());
    strcpy(c9.array, GenerateRandomString(9).c_str());
    strcpy(c10.array, GenerateRandomString(10).c_str());
    strcpy(c20.array, GenerateRandomString(20).c_str());
    strcpy(c30.array, GenerateRandomString(30).c_str());

    for (int i = 0; i < numberOfQueries; i++) {
        int d_id = (i + 1) % 10;
        inputData.C_ID.push_back(customerDataSize + 1 + i);
        inputData.C_D_ID.push_back(d_id);
        inputData.C_W_ID.push_back(1);
        float f = GenerateRandomNumber(1, 100);
        inputData.C_DISCOUNT.push_back(f);
        inputData.C_CREDIT_LIM.push_back(f);
        inputData.C_BALANCE.push_back(f);
        inputData.C_CITY.push_back(c20);
        inputData.C_CREDIT.push_back(c2);
        inputData.C_DATA.push_back(c30);
        inputData.C_FIRST.push_back(c20);
        inputData.C_LAST.push_back(c20);
        inputData.C_MIDDLE.push_back(c2);
        inputData.C_PHONE.push_back(c20);
        charArray10 dateTime;
	strcpy(dateTime.array, CurrentDateTime().c_str());
        inputData.C_SINCE.push_back(dateTime);
        inputData.C_STATE.push_back(c2);
        inputData.C_STREET_1.push_back(c20);
        inputData.C_STREET_2.push_back(c20);
        inputData.C_ZIP.push_back(c9);
        inputData.C_YTD_PAYMENT.push_back(f);
        short int s = GenerateRandomNumber(1, 100);
        inputData.C_DELIVERY_CNT.push_back(s);
        inputData.C_PAYMENT_CNT.push_back(s);
    }

    return inputData;

}

int* QueryDataGenerator::GenerateIndexes(int numberOfQueries) {
    int* inputData = new int[numberOfQueries];
    for (int i = 0; i < numberOfQueries; i++) {
        inputData[i] = GenerateRandomNumber(0, customerDataSize-1);
        //std::cout << inputData[i] << " ";
    }
    sort(inputData, inputData + numberOfQueries);
    
    return inputData;
}

InputDataNewOrderColumnStore QueryDataGenerator::GenerateColumnStoreNewOrderData(int NumberOfQueries, ColumnStoreData columnStoreData) {
    InputDataNewOrderColumnStore inputData;
    int num_of_OL = 0;
    
    for (int i = 0; i < NumberOfQueries; i++) {

        inputData.o_w_id.push_back(1);

        //inputData.w_tax.push_back(columnStoreData.W_TAX[0]);
        int c_id = (i + 1) % 3000;
        if (c_id == 0) {
            c_id = 3000;
        }
        inputData.o_c_id.push_back(c_id);
        int d_id = (i + 1) % 10;
        if (d_id == 0) {
            d_id = 10;
        }
        inputData.o_d_id.push_back(d_id);

        charArray10 dateTime;
        string dt = CurrentDateTime();
        strncpy(dateTime.array, dt.c_str(), 10);
        inputData.o_entry_d.push_back(dateTime);
        int o_id = columnStoreData.D_NEXT_O_ID[d_id - 1] + 1 + i;
        inputData.o_id.push_back(o_id);
        int orderLine_cnt = GenerateRandomNumber(1, 15);
        inputData.o_ol_cnt.push_back(orderLine_cnt);
        int cr_id = GenerateRandomNumber(1, 10);
        inputData.o_carrier_id.push_back(cr_id);
        inputData.o_all_local.push_back(1);

        for (int j = 0; j < orderLine_cnt; j++) {
            num_of_OL++;
            inputData.ol_w_id.push_back(1);
            inputData.ol_d_id.push_back(d_id);
            inputData.ol_o_id.push_back(o_id);
            int id = GenerateRandomNumber(1, 100000);
            inputData.ol_item_id.push_back(id);

            inputData.ol_amount.push_back(columnStoreData.I_PRICE[id - 1]);

            charArray30 distInfo;
            strncpy(distInfo.array, GenerateRandomString(30).c_str(), 30);
            inputData.ol_dist_info.push_back(distInfo);
            inputData.ol_supply_w_id.push_back(1);
            inputData.ol_ol_number.push_back(j + 1);
            inputData.ol_number.push_back(GenerateRandomNumber(1, 15));
            charArray20 nl;
            strncpy(nl.array, GenerateRandomString(20).c_str(), 20);
            inputData.ol_delivery_d.push_back(nl);

            //data.item_price[j] = columnStoreData.I_PRICE[id - 1];//Business Logic
        }

        columnStoreData.D_NEXT_O_ID[d_id - 1] += 1;

    }
    inputData.total_order_lines = num_of_OL;

    return inputData;

}

InputDataNewOrderRowStore QueryDataGenerator::GenerateRowStoreNewOrderData(int queryCount, char** loadedData) {

    InputDataNewOrderRowStore inputData;
    inputData.NewOrder = (char*) malloc(newOrderSize() * queryCount);
    inputData.OrderLine = (char*) malloc(orderLineSize() * queryCount * 15);
    inputData.Orders = (char*) malloc(ordersSize() * queryCount);

    int w_id = 1;
    int global_ol_cnt = 0;
    std::string entry_d = CurrentDateTime();
    for (int i = 0; i < queryCount; i++) {
        short int d_id = (i + 1) % 10;
        if (d_id == 0) {
            d_id = 10;
        }
        int c_id = (i + 1) % 3000;
        if (c_id == 0) {
            c_id = 3000;
        }
        int carrier_id = (i + 1) % 10;
        int ol_cnt = GenerateRandomNumber(5, 15);
        int all_local = 1;
        int o_id = ordersDataSize + 1 + i;

        // Order Data
        //Orders order;
        update_orders(inputData.Orders, i, (char*) &o_id, 0);
        update_orders(inputData.Orders, i, (char*) &d_id, 1);
        update_orders(inputData.Orders, i, (char*) &w_id, 2);
        update_orders(inputData.Orders, i, (char*) &c_id, 3);
        update_orders(inputData.Orders, i, (char*) &carrier_id, 4);
        update_orders(inputData.Orders, i, (char*) &ol_cnt, 5);
        update_orders(inputData.Orders, i, (char*) &all_local, 6);
        update_orders(inputData.Orders, i, (char*) entry_d.c_str(), 7);
        /*order.o_id = o_id;
        order.o_d_id = d_id;
        order.o_w_id = w_id;
        order.o_c_id = c_id;
        order.o_carrier_id = carrier_id;
        order.o_ol_cnt = ol_cnt;
        order.o_all_local = all_local;
        strcpy(order.o_entry_d, entry_d.c_str());
        inputData.o_input.push_back(order);*/

        // New Order Data

        update_new_order(inputData.NewOrder, i, (char*) &o_id, 0);
        update_new_order(inputData.NewOrder, i, (char*) &d_id, 1);
        update_new_order(inputData.NewOrder, i, (char*) &w_id, 2);
        /*New_Order no;
        no.no_o_id = o_id;
        no.no_d_id = d_id;
        no.no_w_id = w_id;
        inputData.no_input.push_back(no);*/

        // End



        for (int i = 0; i < ol_cnt; i++) {

            int item_id = GenerateRandomNumber(1, 100000);
            float item_price = *read_item(loadedData[3], item_id - 1, 2);
            float customer_disc = *read_customer(loadedData[1], ((d_id - 1) * 3000) + c_id, 3);
            float d_tax = *read_district(loadedData[0], d_id, 4);
            float w_tax = *read_warehouse(loadedData[7], 0, 2);
            float disc_price = item_price - (item_price * customer_disc);
            //float calculated_price = disc_price + (d_tax*disc_price) + (w_tax*disc_price);
            string distinfo = GenerateRandomString(30);

            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &o_id, 0);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &d_id, 1);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &w_id, 2);
            int ol_num = GenerateRandomNumber(1, 15);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &ol_num, 3);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &item_id, 4);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) entry_d.c_str(), 5);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &item_price, 6);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &w_id, 7);
            int ol_quantity = 5;
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) &ol_quantity, 8);
            update_order_line(inputData.OrderLine, global_ol_cnt, (char*) distinfo.c_str(), 9);

            /*Order_Line ol;
            ol.ol_o_id = o_id;
            ol.ol_d_id = d_id;
            ol.ol_w_id = w_id;
            ol.ol_number = GenerateRandomNumber(1, 15);
            ol.ol_i_id = item_id;
            strcpy(ol.ol_delivery_d , entry_d.c_str());
            ol.ol_amount = item_price;
            ol.ol_supply_w_id = w_id;
            ol.ol_quantity = 5;
            strcpy(ol.ol_dist_info, distinfo.c_str());
            inputData.ol_input.push_back(ol);*/

            global_ol_cnt++;
        }
    }
    char* OrderLine2 = (char*) malloc(orderLineSize() * global_ol_cnt);
    memcpy(OrderLine2, inputData.OrderLine, orderLineSize() * global_ol_cnt);
    free(inputData.OrderLine);
    inputData.OrderLine = OrderLine2;
    inputData.total_order_lines = global_ol_cnt;
    return inputData;
}

