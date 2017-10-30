#pragma once

#include<vector>
#include <cstring>
#include<fstream>
#include<sstream>
#include<iostream>
#include "Column_Store_Data.h"

class Column_Store_Import_methods
{
public:
	Column_Store_Import_methods();
	~Column_Store_Import_methods();
	vector<string> FillCommaSeparatedValues(string path);

	void ImportColumnMajorData_District(ColumnStoreData *columnStoreData);
	void ImportColumnMajorData_Customer(ColumnStoreData *columnStoreData);

	void ImportColumnMajorData_History(ColumnStoreData *columnStoreData);

	void ImportColumnMajorData_Item(ColumnStoreData *columnStoreData);

	void ImportColumnMajorData_NewOrder(ColumnStoreData *columnStoreData);

	void ImportColumnMajorData_OrderLine(ColumnStoreData *columnStoreData);

	void ImportColumnMajorData_Orders(ColumnStoreData *columnStoreData);

	void ImportColumnMajorData_Stock(ColumnStoreData *columnStoreData);

	void ImportColumnMajorData_Warehouse(ColumnStoreData *columnStoreData);
};

