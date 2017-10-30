#ifndef COLUMNSTOREDATA_H
#define COLUMNSTOREDATA_H
#include <vector>
using namespace std;

struct charArray2{
    char array[2];
};

struct charArray9{
    char array[9];
};

struct charArray10{
    char array[10];
};

struct charArray20{
    char array[20];
};

struct charArray30{
    char array[30];
};

struct ColumnStoreData {
    //CUSTOMER
    std::vector<int> C_ID;
    std::vector<int> C_D_ID;
    std::vector<int> C_W_ID;
    std::vector<float> C_DISCOUNT;
    std::vector<float> C_CREDIT_LIM;
    std::vector<charArray20> C_LAST;
    std::vector<charArray20> C_FIRST;
    std::vector<charArray2> C_CREDIT;
    std::vector<float> C_BALANCE;
    std::vector<float> C_YTD_PAYMENT;
    std::vector<int> C_PAYMENT_CNT;
    std::vector<int> C_DELIVERY_CNT;
    std::vector<charArray20> C_STREET_1;
    std::vector<charArray20> C_STREET_2;
    std::vector<charArray20> C_CITY;
    std::vector<charArray2> C_STATE;
    std::vector<charArray9> C_ZIP;
    std::vector<charArray20> C_PHONE;
    std::vector<charArray10> C_SINCE;
    std::vector<charArray2> C_MIDDLE;
    std::vector<charArray30> C_DATA;
    //    

    //     //District
    std::vector<int> D_ID;
    std::vector<int> D_W_ID;
    std::vector<float> D_YTD;
    std::vector<int> D_NEXT_O_ID;
    std::vector<float> D_TAX;
    std::vector<charArray30> D_NAME;
    std::vector<charArray30> D_STREET_1;
    std::vector<charArray30> D_STREET_2;
    std::vector<charArray30> D_CITY;
    std::vector<charArray30> D_STATE;
    std::vector<charArray30> D_ZIP;
    //
    ////History
    std::vector<int> H_C_ID;
    std::vector<int> H_C_D_ID;
    std::vector<int> H_C_W_ID;
    std::vector<int> H_D_ID;
    std::vector<int> H_W_ID;
    std::vector<charArray30> H_DATE;
    std::vector<float> H_AMOUNT;
    std::vector<charArray30> H_DATA;
    //    
    //  //Item
    std::vector<int> I_ID;
    std::vector<charArray30> I_NAME;
    std::vector<float> I_PRICE;
    std::vector<charArray30> I_DATA;
    std::vector<int> I_IM_ID;
    //    
    //    //New Order
    std::vector<int> NO_O_ID;
    std::vector<int> NO_D_ID;
    std::vector<int> NO_W_ID;
    //    
    //    //Order Line
    std::vector<int> OL_O_ID;
    std::vector<int> OL_D_ID;
    std::vector<int> OL_W_ID;
    std::vector<int> OL_NUMBER;
    std::vector<int> OL_I_ID;
    std::vector<charArray30> OL_DELIVERY_D;
    std::vector<float> OL_AMOUNT;
    std::vector<int> OL_SUPPLY_W_ID;
    std::vector<int> OL_QUANTITY;
    std::vector<charArray30> OL_DIST_INFO;
    //    
    //    //Orders
    std::vector<int> O_ID;
    std::vector<int> O_D_ID;
    std::vector<int> O_W_ID;
    std::vector<int> O_C_ID;
    std::vector<int> O_CARRIER_ID;
    std::vector<int> O_OL_CNT;
    std::vector<int> O_ALL_LOCAL;
    std::vector<charArray30> O_ENTRY_D;
    //    
    //    //Stock
    std::vector<int> S_I_ID;
    std::vector<int> S_W_ID;
    std::vector<int> S_QUANTITY;
    std::vector<int> S_YTD;
    std::vector<int> S_ORDER_CNT;
    std::vector<int> S_REMOTE_CNT;
    std::vector<charArray30> S_DATA;
    std::vector<charArray30> S_DIST_01;
    std::vector<charArray30> S_DIST_02;
    std::vector<charArray30> S_DIST_03;
    std::vector<charArray30> S_DIST_04;
    std::vector<charArray30> S_DIST_05;
    std::vector<charArray30> S_DIST_06;
    std::vector<charArray30> S_DIST_07;
    std::vector<charArray30> S_DIST_08;
    std::vector<charArray30> S_DIST_09;
    std::vector<charArray30> S_DIST_10;
    //    
    //    //WareHouse
    std::vector<int> W_ID;
    std::vector<float> W_YTD;
    std::vector<float> W_TAX;
    std::vector<charArray30> W_NAME;
    std::vector<charArray30> W_STREET_1;
    std::vector<charArray30> W_STREET_2;
    std::vector<charArray30> W_CITY;
    std::vector<charArray30> W_STATE;
    std::vector<charArray30> W_ZIP;

};

struct InputDataInsertColumnStore {
    std::vector<int> C_ID;
    std::vector<int> C_D_ID;
    std::vector<int> C_W_ID;
    std::vector<float> C_DISCOUNT;
    std::vector<float> C_CREDIT_LIM;
    std::vector<charArray20> C_LAST;
    std::vector<charArray20> C_FIRST;
    std::vector<charArray2> C_CREDIT;
    std::vector<float> C_BALANCE;
    std::vector<float> C_YTD_PAYMENT;
    std::vector<int> C_PAYMENT_CNT;
    std::vector<int> C_DELIVERY_CNT;
    std::vector<charArray20> C_STREET_1;
    std::vector<charArray20> C_STREET_2;
    std::vector<charArray20> C_CITY;
    std::vector<charArray2> C_STATE;
    std::vector<charArray9> C_ZIP;
    std::vector<charArray20> C_PHONE;
    std::vector<charArray10> C_SINCE;
    std::vector<charArray2> C_MIDDLE;
    std::vector<charArray30> C_DATA;
};

struct InputDataNewOrderColumnStore
{
	vector<int> o_w_id;
	vector<int> o_d_id;
	vector<int> o_c_id;
	vector<int> o_id;
	vector<int> o_carrier_id;
	vector<int> o_ol_cnt;
	vector<int> o_all_local;
	vector<charArray10> o_entry_d;

	vector<charArray30> ol_dist_info;
	vector<int>	ol_o_id;
	vector<int>	ol_d_id;
	vector<int>	ol_w_id;
	vector<int>	ol_number;
	vector<int>	ol_item_id;
        vector<charArray20> ol_delivery_d;
	vector<float> ol_amount;
	vector<int> ol_supply_w_id;
	vector<int>	ol_ol_number;
	int total_order_lines;

};

#endif
