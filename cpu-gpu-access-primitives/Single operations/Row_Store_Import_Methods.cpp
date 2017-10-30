#include "Row_Store_Import_Methods.h"

extern int ordersDataSize;
extern int customerDataSize;

using namespace std;

string filepath = "/home/arefeva/NBP/Thesis Data/";

Row_Store_Import_Methods::Row_Store_Import_Methods() {
}

Row_Store_Import_Methods::~Row_Store_Import_Methods() {
}

vector<string> Row_Store_Import_Methods::FillCommaSeparatedValues(string path) {
    string line;
    vector<string> str_Data;
    ifstream myfile;
    myfile.open(path.c_str(), ios::in);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            std::istringstream ss(line);
            //cout<<line<<endl;
            std::string token;
            //cout<<line<<endl;
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) {
                    str_Data.push_back(token);
                }
            }
        }

    } else {
        cout << "Unable to open file";
    }
    myfile.close();
    return str_Data;

}

void Row_Store_Import_Methods::ImportRowMajorData_District(char** loadedData, int pos) {
    string path = filepath + "District.csv";
    vector<string> str_dist_Data;

    str_dist_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(districtSize() * ((str_dist_Data.size() - 11) / 11));

    for (int i = 1; i < ((str_dist_Data.size() - 11) / 11) + 1; i++) {
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11].c_str(), 0);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 1].c_str(), 1);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 2].c_str(), 2);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 3].c_str(), 3);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 4].c_str(), 4);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 5].c_str(), 5);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 6].data(), 6);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 7].data(), 7);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 8].data(), 8);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 9].c_str(), 9);
        update_district(loadedData[pos], i - 1, (char*)str_dist_Data[i * 11 + 10].c_str(), 10);

    }

}

void Row_Store_Import_Methods::ImportRowMajorData_Customer(char **loadedData, int pos) {
    string path = filepath + "Customer.csv";
    vector<string> str_cust_Data;

    str_cust_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(customerSize() * ((str_cust_Data.size() - 21) / 21));
    customerDataSize = (str_cust_Data.size() - 21) / 21;

    for (int i = 1; i < ((str_cust_Data.size() - 21) / 21) + 1; i++) {

        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21].c_str(), 0);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 1].c_str(), 1);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 2].c_str(), 2);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 3].c_str(), 3);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 4].c_str(), 4);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 5].data(), 5);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 6].data(), 6);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 7].c_str(), 7);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 8].c_str(), 8);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 9].c_str(), 9);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 10].c_str(), 10);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 11].c_str(), 11);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 12].data(), 12);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 13].data(), 13);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 14].data(), 14);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 15].data(), 15);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 16].data(), 16);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 17].data(), 17);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 18].data(), 18);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 19].data(), 19);
        update_customer(loadedData[pos], i - 1, (char*)str_cust_Data[i * 21 + 20].data(), 20);
    }

}

void Row_Store_Import_Methods::ImportRowMajorData_History(char **loadedData, int pos) {
    string path = filepath + "History.csv";
    vector<string> str_hist_Data;

    str_hist_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(historySize() * ((str_hist_Data.size() - 8) / 8));

    for (int i = 1; i < ((str_hist_Data.size() - 8) / 8) + 1; i++) {

        update_history(loadedData[pos], i - 1, (char*)str_hist_Data[i * 8].c_str(), 0);
        update_history(loadedData[pos], i - 1, (char*)(str_hist_Data[i * 8 + 1].c_str()), 1);
        update_history(loadedData[pos], i - 1, (char*)(str_hist_Data[i * 8 + 2].c_str()), 2);
        update_history(loadedData[pos], i - 1, (char*)(str_hist_Data[i * 8 + 3].c_str()), 3);
        update_history(loadedData[pos], i - 1, (char*)(str_hist_Data[i * 8 + 4].c_str()), 4);
        update_history(loadedData[pos], i - 1, (char*)str_hist_Data[i * 8 + 5].c_str(), 5);
        update_history(loadedData[pos], i - 1, (char*)str_hist_Data[i * 8 + 6].c_str(), 6);
        update_history(loadedData[pos], i - 1, (char*)str_hist_Data[i * 8 + 7].c_str(), 7);
    }

}

void Row_Store_Import_Methods::ImportRowMajorData_Item(char **loadedData, int pos) {
    string path = filepath + "ITEM.csv";
    vector<string> str_item_Data;

    str_item_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(itemSize() * ((str_item_Data.size() - 5) / 5));

    for (unsigned int i = 1; i < ((str_item_Data.size() - 5) / 5) + 1; i++) {

        update_item(loadedData[pos], i - 1, (char*)(str_item_Data[i * 5].c_str()), 0);
        update_item(loadedData[pos], i - 1, (char*)str_item_Data[i * 5 + 1].c_str(), 1);
        update_item(loadedData[pos], i - 1, (char*)(str_item_Data[i * 5 + 2].c_str()), 2);
        update_item(loadedData[pos], i - 1, (char*)str_item_Data[i * 5 + 3].c_str(), 3);
        update_item(loadedData[pos], i - 1, (char*)(str_item_Data[i * 5 + 4].c_str()), 4);

    }

}

void Row_Store_Import_Methods::ImportRowMajorData_NewOrder(char **loadedData, int pos) {
    string path = filepath + "NEW_ORDER.csv";
    vector<string> str_no_Data;
    //vector<New_Order> no_Data;
    str_no_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(newOrderSize() * ((str_no_Data.size() - 3) / 3));

    for (unsigned int i = 1; i < ((str_no_Data.size() - 3) / 3) + 1; i++) {

        update_new_order(loadedData[pos], i - 1, (char*)(str_no_Data[i * 3].c_str()), 0);
        update_new_order(loadedData[pos], i - 1, (char*)(str_no_Data[i * 3 + 1].c_str()), 1);
        update_new_order(loadedData[pos], i - 1, (char*)(str_no_Data[i * 3 + 1].c_str()), 2);

    }
}

void Row_Store_Import_Methods::ImportRowMajorData_OrderLine(char **loadedData, int pos) {
    string path = filepath + "ORDER_LINE.csv";
    vector<string> str_ol_Data;
    str_ol_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(orderLineSize() * ((str_ol_Data.size() - 10) / 10));

    for (unsigned int i = 1; i < ((str_ol_Data.size() - 10) / 10) + 1; i++) {

        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10].c_str()), 0);
        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10 + 1].c_str()), 1);
        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10 + 2].c_str()), 2);
        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10 + 3].c_str()), 3);
        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10 + 4].c_str()), 4);
        update_order_line(loadedData[pos], i - 1, (char*)str_ol_Data[i * 10 + 5].c_str(), 5);
        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10 + 6].c_str()), 6);
        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10 + 7].c_str()), 7);
        update_order_line(loadedData[pos], i - 1, (char*)(str_ol_Data[i * 10 + 8].c_str()), 8);
        update_order_line(loadedData[pos], i - 1, (char*)str_ol_Data[i * 10 + 9].c_str(), 9);
    }
}

void Row_Store_Import_Methods::ImportRowMajorData_Orders(char **loadedData, int pos) {
    string path = filepath + "ORDERS.csv";
    vector<string> str_orders_Data;

    str_orders_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(ordersSize() * ((str_orders_Data.size() - 8) / 8));
    ordersDataSize = (str_orders_Data.size() - 8) / 8;

    for (unsigned int i = 1; i < ((str_orders_Data.size() - 8) / 8) + 1; i++) {

        update_orders(loadedData[pos], i - 1, (char*)(str_orders_Data[i * 8].c_str()), 0);
        update_orders(loadedData[pos], i - 1, (char*)(str_orders_Data[i * 8 + 1].c_str()), 1);
        update_orders(loadedData[pos], i - 1, (char*)(str_orders_Data[i * 8 + 2].c_str()), 2);
        update_orders(loadedData[pos], i - 1, (char*)(str_orders_Data[i * 8 + 3].c_str()), 3);
        update_orders(loadedData[pos], i - 1, (char*)(str_orders_Data[i * 8 + 4].c_str()), 4);
        update_orders(loadedData[pos], i - 1, (char*)(str_orders_Data[i * 8 + 5].c_str()), 5);
        update_orders(loadedData[pos], i - 1, (char*)(str_orders_Data[i * 8 + 6].c_str()), 6);
        update_orders(loadedData[pos], i - 1, (char*)str_orders_Data[i * 8 + 7].c_str(), 7);

    }

}

void Row_Store_Import_Methods::ImportRowMajorData_Stock(char **loadedData, int pos) {
    string path = filepath + "STOCK.csv";
    vector<string> str_stock_Data;

    str_stock_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(stockSize() * ((str_stock_Data.size() - 17) / 17));

    for (unsigned int i = 1; i < ((str_stock_Data.size() - 17) / 17) + 1; i++) {

        update_stock(loadedData[pos], i - 1, (char*)(str_stock_Data[i * 17].c_str()), 0);
        update_stock(loadedData[pos], i - 1, (char*)(str_stock_Data[i * 17 + 1].c_str()), 1);
        update_stock(loadedData[pos], i - 1, (char*)(str_stock_Data[i * 17 + 2].c_str()), 2);
        update_stock(loadedData[pos], i - 1, (char*)(str_stock_Data[i * 17 + 3].c_str()), 3);
        update_stock(loadedData[pos], i - 1, (char*)(str_stock_Data[i * 17 + 4].c_str()), 4);
        update_stock(loadedData[pos], i - 1, (char*)(str_stock_Data[i * 17 + 5].c_str()), 5);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 6].c_str(), 6);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 7].c_str(), 7);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 8].c_str(), 8);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 9].c_str(), 9);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 10].c_str(), 10);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 11].c_str(), 11);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 12].c_str(), 12);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 13].c_str(), 13);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 14].c_str(), 14);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 15].c_str(), 15);
        update_stock(loadedData[pos], i - 1, (char*)str_stock_Data[i * 17 + 16].c_str(), 16);

    }

}

void Row_Store_Import_Methods::ImportRowMajorData_Warehouse(char **loadedData, int pos) {
    string path = filepath + "WAREHOUSE.csv";
    vector<string> str_wh_Data;

    str_wh_Data = FillCommaSeparatedValues(path);

    loadedData[pos] = (char*) malloc(warehouseSize() * ((str_wh_Data.size() - 9) / 9));

    for (unsigned int i = 1; i < ((str_wh_Data.size() - 9) / 9) + 1; i++) {

        update_warehouse(loadedData[pos], i - 1, (char*)(str_wh_Data[i * 9].c_str()), 0);
        update_warehouse(loadedData[pos], i - 1, (char*)(str_wh_Data[i * 9 + 1].c_str()), 1);
        update_warehouse(loadedData[pos], i - 1, (char*)(str_wh_Data[i * 9 + 2].c_str()), 2);
        update_warehouse(loadedData[pos], i - 1, (char*)str_wh_Data[i * 9 + 3].c_str(), 3);
        update_warehouse(loadedData[pos], i - 1, (char*)str_wh_Data[i * 9 + 4].c_str(), 4);
        update_warehouse(loadedData[pos], i - 1, (char*)str_wh_Data[i * 9 + 5].c_str(), 5);
        update_warehouse(loadedData[pos], i - 1, (char*)str_wh_Data[i * 9 + 6].c_str(), 6);
        update_warehouse(loadedData[pos], i - 1, (char*)str_wh_Data[i * 9 + 7].c_str(), 7);
        update_warehouse(loadedData[pos], i - 1, (char*)str_wh_Data[i * 9 + 8].c_str(), 8);

    }

}


