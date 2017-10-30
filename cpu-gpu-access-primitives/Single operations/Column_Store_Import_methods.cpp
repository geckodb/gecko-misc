
#include "Column_Store_Import_methods.h"


//string filepaths = "F:\\SAP_LAPTOP_Backup\\OpenCL\\Visual studio 2015\\Projects\\Data_CSV\\";
Column_Store_Import_methods::Column_Store_Import_methods()
{
}


Column_Store_Import_methods::~Column_Store_Import_methods()
{
}

using namespace std;


vector<string> Column_Store_Import_methods::FillCommaSeparatedValues(string path)
{
	string line;
	vector<string> str_Data;
	ifstream myfile;
	myfile.open(path.c_str(), ios::in);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			std::istringstream ss(line);
			//cout<<line<<endl;
			std::string token;
			//cout<<line<<endl;
			while (std::getline(ss, token, ','))
			{
				if (!token.empty())
				{
					str_Data.push_back(token);
				}
			}
		}
	}
	else
	{
		cout << "Unable to open file";
	}
	myfile.close();
	return str_Data;

}


void Column_Store_Import_methods::ImportColumnMajorData_District(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/District.csv";
	vector<string> str_dist_Data;

	str_dist_Data = FillCommaSeparatedValues(path);
	for (int i = 1; i<((str_dist_Data.size() - 11) / 11) + 1; i++)
	{
		columnStoreData->D_ID.push_back(atoi(str_dist_Data[i * 11].c_str()));
		columnStoreData->D_W_ID.push_back(atoi(str_dist_Data[i * 11 + 1].c_str()));
		columnStoreData->D_YTD.push_back(atof(str_dist_Data[i * 11 + 2].c_str()));
		columnStoreData->D_NEXT_O_ID.push_back(atoi(str_dist_Data[i * 11 + 3].c_str()));
		columnStoreData->D_TAX.push_back(atof(str_dist_Data[i * 11 + 4].c_str()));


		charArray30 name ;
		std::strcpy(name.array, str_dist_Data[i * 11 + 5].c_str());
		columnStoreData->D_NAME.push_back(name);
		
		charArray30 street1;
		std::strcpy(street1.array, str_dist_Data[i * 11 + 6].c_str());
		columnStoreData->D_STREET_1.push_back(street1);

		charArray30 street2;
		std::strcpy(street2.array, str_dist_Data[i * 11 + 7].c_str());
		columnStoreData->D_STREET_2.push_back(street2);

		charArray30 city;
		std::strcpy(city.array, str_dist_Data[i * 11 + 8].c_str());
		columnStoreData->D_CITY.push_back(city);

		charArray30 state;
		std::strcpy(state.array, str_dist_Data[i * 11 + 9].c_str());
		columnStoreData->D_STATE.push_back(state);

		charArray30 zip;
		std::strcpy(zip.array, str_dist_Data[i * 11 + 10].c_str());
		columnStoreData->D_ZIP.push_back(zip);

	}
}


void Column_Store_Import_methods::ImportColumnMajorData_Customer(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/Customer.csv";
	vector<string> str_cust_Data;
	str_cust_Data = FillCommaSeparatedValues(path);

	for (int i = 1; i<((str_cust_Data.size() - 21) / 21) + 1; i++)
	{

		columnStoreData->C_ID.push_back(atoi(str_cust_Data[i * 21].c_str()));
		columnStoreData->C_D_ID.push_back(atoi(str_cust_Data[i * 21 + 1].c_str()));
		columnStoreData->C_W_ID.push_back(atoi(str_cust_Data[i * 21 + 2].c_str()));
		columnStoreData->C_DISCOUNT.push_back(atof(str_cust_Data[i * 21 + 3].c_str()));
		columnStoreData->C_CREDIT_LIM.push_back(atof(str_cust_Data[i * 21 + 4].c_str()));

		charArray20 last;
		std::strcpy(last.array, str_cust_Data[i * 21 + 5].c_str());
		columnStoreData->C_LAST.push_back(last);

		charArray20 first;
		std::strcpy(first.array, str_cust_Data[i * 21 + 6].c_str());
		columnStoreData->C_FIRST.push_back(first);
		
		charArray2 credit;
		std::strcpy(credit.array, str_cust_Data[i * 21 + 7].c_str());
		columnStoreData->C_CREDIT.push_back(credit);

		columnStoreData->C_BALANCE.push_back(atof(str_cust_Data[i * 21 + 8].c_str()));
		columnStoreData->C_YTD_PAYMENT.push_back(atof(str_cust_Data[i * 21 + 9].c_str()));
		columnStoreData->C_PAYMENT_CNT.push_back(atoi(str_cust_Data[i * 21 + 10].c_str()));

		columnStoreData->C_DELIVERY_CNT.push_back(atoi(str_cust_Data[i * 21 + 11].c_str()));

		charArray20 street1 ;
		std::strcpy(street1.array, str_cust_Data[i * 21 + 12].c_str());
		columnStoreData->C_STREET_1.push_back(street1);

		charArray20 street2 ;
		std::strcpy(street2.array, str_cust_Data[i * 21 + 13].c_str());
		columnStoreData->C_STREET_2.push_back(street2);
		
		charArray20 city;
		std::strcpy(city.array, str_cust_Data[i * 21 + 14].c_str());
		columnStoreData->C_CITY.push_back(city);

		charArray2 state ;
		std::strcpy(state.array, str_cust_Data[i * 21 + 15].c_str());
		columnStoreData->C_STATE.push_back(state);
		
		charArray9 zip ;
		std::strcpy(zip.array, str_cust_Data[i * 21 + 16].c_str());
		columnStoreData->C_ZIP.push_back(zip);

		charArray20 phone ;
		std::strcpy(phone.array, str_cust_Data[i * 21 + 17].c_str());
		columnStoreData->C_PHONE.push_back(phone);

		charArray10 since ;
		std::strcpy(since.array, str_cust_Data[i * 21 + 18].c_str());
		columnStoreData->C_SINCE.push_back(since);

		charArray2 middle_Name;
		std::strcpy(middle_Name.array, str_cust_Data[i * 21 + 19].c_str());
		columnStoreData->C_MIDDLE.push_back(middle_Name);
		
		charArray30 data;
		std::strncpy(data.array, str_cust_Data[i * 21 + 20].c_str(),29);
                data.array[29] = '\0';
		columnStoreData->C_DATA.push_back(data);
	}

}

void Column_Store_Import_methods::ImportColumnMajorData_History(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/History.csv";
	vector<string> str_hist_Data;
	str_hist_Data = FillCommaSeparatedValues(path);
	for (int i = 1; i<((str_hist_Data.size() - 8) / 8) + 1; i++)
	{
		columnStoreData->H_C_ID.push_back(atoi(str_hist_Data[i * 8].c_str()));
		columnStoreData->H_C_D_ID.push_back(atoi(str_hist_Data[i * 8 + 1].c_str()));
		columnStoreData->H_C_W_ID.push_back(atoi(str_hist_Data[i * 8 + 2].c_str()));
		columnStoreData->H_D_ID.push_back(atoi(str_hist_Data[i * 8 + 3].c_str()));
		columnStoreData->H_W_ID.push_back(atoi(str_hist_Data[i * 8 + 4].c_str()));

		charArray30 date;
		std::strcpy(date.array, str_hist_Data[i * 8 + 5].c_str());
		columnStoreData->H_DATE.push_back(date);

		
		columnStoreData->H_AMOUNT.push_back(atof(str_hist_Data[i * 8 + 6].c_str()));

		charArray30 data;
		std::strcpy(data.array, str_hist_Data[i * 8 + 7].c_str());
		columnStoreData->H_DATE.push_back(data);
		

	}
}

void Column_Store_Import_methods::ImportColumnMajorData_Item(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/ITEM.csv";
	vector<string> str_item_Data;
	str_item_Data = FillCommaSeparatedValues(path);

	for (unsigned int i = 1; i<((str_item_Data.size() - 5) / 5) + 1; i++)
	{
		columnStoreData->I_ID.push_back(atoi(str_item_Data[i * 5].c_str()));

		charArray30 name ;
		std::strcpy(name.array, str_item_Data[i * 5 + 1].c_str());
		columnStoreData->I_NAME.push_back(name);
		
		columnStoreData->I_PRICE.push_back(atof(str_item_Data[i * 5 + 2].c_str()));

		charArray30 data;
		std::strncpy(data.array, str_item_Data[i * 5 + 3].c_str(),30);
		columnStoreData->I_DATA.push_back(data);
		
		columnStoreData->I_IM_ID.push_back(atoi(str_item_Data[i * 5 + 4].c_str()));
	}
}

void Column_Store_Import_methods::ImportColumnMajorData_NewOrder(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/NEW_ORDER.csv";
	vector<string> str_no_Data;
	str_no_Data = FillCommaSeparatedValues(path);
	for (unsigned int i = 1; i<((str_no_Data.size() - 3) / 3) + 1; i++)
	{
		columnStoreData->NO_O_ID.push_back(atoi(str_no_Data[i * 3].c_str()));
		columnStoreData->NO_D_ID.push_back(atoi(str_no_Data[i * 3 + 1].c_str()));
		columnStoreData->NO_W_ID.push_back(atoi(str_no_Data[i * 3 + 1].c_str()));

	}
}

void Column_Store_Import_methods::ImportColumnMajorData_OrderLine(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/ORDER_LINE.csv";
	vector<string> str_ol_Data;
	str_ol_Data = FillCommaSeparatedValues(path);
	for (unsigned int i = 1; i<((str_ol_Data.size() - 10) / 10) + 1; i++)
	{
		columnStoreData->OL_O_ID.push_back(atoi(str_ol_Data[i * 10].c_str()));
		columnStoreData->OL_D_ID.push_back(atoi(str_ol_Data[i * 10 + 1].c_str()));
		columnStoreData->OL_W_ID.push_back(atoi(str_ol_Data[i * 10 + 2].c_str()));
		columnStoreData->OL_NUMBER.push_back(atoi(str_ol_Data[i * 10 + 3].c_str()));
		columnStoreData->OL_I_ID.push_back(atoi(str_ol_Data[i * 10 + 4].c_str()));
		charArray30 delevery_d;
		std::strcpy(delevery_d.array, str_ol_Data[i * 10 + 5].c_str());
		columnStoreData->OL_DELIVERY_D.push_back(delevery_d);
		
		columnStoreData->OL_AMOUNT.push_back(atof(str_ol_Data[i * 10 + 6].c_str()));
		columnStoreData->OL_SUPPLY_W_ID.push_back(atoi(str_ol_Data[i * 10 + 7].c_str()));
		columnStoreData->OL_QUANTITY.push_back(atoi(str_ol_Data[i * 10 + 8].c_str()));

		charArray30 dist_info;
		std::strncpy(dist_info.array, str_ol_Data[i * 10 + 9].c_str(),30);
		columnStoreData->OL_DIST_INFO.push_back(dist_info);
		
	}
}

void Column_Store_Import_methods::ImportColumnMajorData_Orders(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/ORDERS.csv";
	vector<string> str_orders_Data;
	str_orders_Data = FillCommaSeparatedValues(path);
	for (unsigned int i = 1; i<((str_orders_Data.size() - 8) / 8) + 1; i++)
	{
		columnStoreData->O_ID.push_back(atoi(str_orders_Data[i * 8].c_str()));
		columnStoreData->O_D_ID.push_back(atoi(str_orders_Data[i * 8 + 1].c_str()));
		columnStoreData->O_W_ID.push_back(atoi(str_orders_Data[i * 8 + 2].c_str()));
		columnStoreData->O_C_ID.push_back(atoi(str_orders_Data[i * 8 + 3].c_str()));
		columnStoreData->O_CARRIER_ID.push_back(atoi(str_orders_Data[i * 8 + 4].c_str()));
		columnStoreData->O_OL_CNT.push_back(atoi(str_orders_Data[i * 8 + 5].c_str()));
		columnStoreData->O_ALL_LOCAL.push_back(atoi(str_orders_Data[i * 8 + 6].c_str()));

		charArray30 entry_d ;
		std::strcpy(entry_d.array, str_orders_Data[i * 8 + 7].c_str());
		columnStoreData->O_ENTRY_D.push_back(entry_d);
	}
}

void Column_Store_Import_methods::ImportColumnMajorData_Stock(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/STOCK.csv";
	vector<string> str_stock_Data;
	str_stock_Data = FillCommaSeparatedValues(path);
	for (unsigned int i = 1; i<((str_stock_Data.size() - 17) / 17) + 1; i++)
	{
		columnStoreData->S_I_ID.push_back(atoi(str_stock_Data[i * 17].c_str()));
		columnStoreData->S_W_ID.push_back(atoi(str_stock_Data[i * 17 + 1].c_str()));
		columnStoreData->S_QUANTITY.push_back(atoi(str_stock_Data[i * 17 + 2].c_str()));
		columnStoreData->S_YTD.push_back(atoi(str_stock_Data[i * 17 + 3].c_str()));
		columnStoreData->S_ORDER_CNT.push_back(atoi(str_stock_Data[i * 17 + 4].c_str()));
		columnStoreData->S_REMOTE_CNT.push_back(atoi(str_stock_Data[i * 17 + 5].c_str()));

		charArray30 data ;
		std::strcpy(data.array, str_stock_Data[i * 17 + 6].c_str());
		columnStoreData->S_DATA.push_back(data);

		charArray30 district1 ;
		std::strcpy(district1.array, str_stock_Data[i * 17 + 7].c_str());
		columnStoreData->S_DIST_01.push_back(district1);

		charArray30 district2 ;
		std::strcpy(district2.array, str_stock_Data[i * 17 + 8].c_str());
		columnStoreData->S_DIST_02.push_back(district2);

		charArray30 district3 ;
		std::strcpy(district3.array, str_stock_Data[i * 17 + 9].c_str());
		columnStoreData->S_DIST_03.push_back(district3);

		charArray30 district4 ;
		std::strcpy(district4.array, str_stock_Data[i * 17 + 10].c_str());
		columnStoreData->S_DIST_04.push_back(district4);

		charArray30 district5;
		std::strcpy(district5.array, str_stock_Data[i * 17 + 11].c_str());
		columnStoreData->S_DIST_05.push_back(district5);

		charArray30 district6 ;
		std::strcpy(district6.array, str_stock_Data[i * 17 + 12].c_str());
		columnStoreData->S_DIST_06.push_back(district6);

		charArray30 district7 ;
		std::strcpy(district7.array, str_stock_Data[i * 17 + 13].c_str());
		columnStoreData->S_DIST_07.push_back(district7);

		charArray30 district8;
		std::strcpy(district8.array, str_stock_Data[i * 17 + 14].c_str());
		columnStoreData->S_DIST_08.push_back(district8);

		charArray30 district9;
		std::strcpy(district9.array, str_stock_Data[i * 17 + 15].c_str());
		columnStoreData->S_DIST_09.push_back(district9);

		charArray30 district10 ;
		std::strcpy(district10.array, str_stock_Data[i * 17 + 16].c_str());
		columnStoreData->S_DIST_10.push_back(district10);
	}
}

void Column_Store_Import_methods::ImportColumnMajorData_Warehouse(ColumnStoreData *columnStoreData)
{
	string path = "/home/arefeva/NBP/Thesis Data/WAREHOUSE.csv";
	vector<string> str_wh_Data;
	str_wh_Data = FillCommaSeparatedValues(path);
	for (unsigned int i = 1; i<((str_wh_Data.size() - 9) / 9) + 1; i++)
	{
		columnStoreData->W_ID.push_back(atoi(str_wh_Data[i * 9].c_str()));
		columnStoreData->W_YTD.push_back(atof(str_wh_Data[i * 9 + 1].c_str()));
		columnStoreData->W_TAX.push_back(atof(str_wh_Data[i * 9 + 2].c_str()));

		charArray30 name ;
		std::strcpy(name.array, str_wh_Data[i * 9 + 3].c_str());
		columnStoreData->W_NAME.push_back(name);

		charArray30 street1 ;
		std::strcpy(street1.array, str_wh_Data[i * 9 + 4].c_str());
		columnStoreData->W_STREET_1.push_back(street1);

		charArray30 street2 ;
		std::strcpy(street2.array, str_wh_Data[i * 9 + 5].c_str());
		columnStoreData->W_STREET_2.push_back(street2);

		charArray30 city;
		std::strcpy(city.array, str_wh_Data[i * 9 + 6].c_str());
		columnStoreData->W_CITY.push_back(city);

		charArray30 state ;
		std::strcpy(state.array, str_wh_Data[i * 9 + 7].c_str());
		columnStoreData->W_STATE.push_back(state);

		charArray30 zip ;
		std::strcpy(zip.array, str_wh_Data[i * 9 + 8].c_str());
		columnStoreData->W_ZIP.push_back(zip);

	}

}
