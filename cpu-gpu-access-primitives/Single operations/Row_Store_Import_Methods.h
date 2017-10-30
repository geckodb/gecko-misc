

#pragma once
#include<vector>
#include<fstream>
#include <cstring>
#include<sstream>
#include<iostream>
#include "Row_Store_Data.h"
using namespace std;
class Row_Store_Import_Methods
{
public:
	Row_Store_Import_Methods();
	~Row_Store_Import_Methods();

	vector<string> FillCommaSeparatedValues(string path);
	

	void ImportRowMajorData_District(char **loadedData, int pos);

	void ImportRowMajorData_Customer(char **loadedData, int pos);

	void ImportRowMajorData_History(char **loadedData, int pos);

	void ImportRowMajorData_Item(char **loadedData, int pos);

	void ImportRowMajorData_NewOrder(char **loadedData, int pos);

	void ImportRowMajorData_OrderLine(char **loadedData, int pos);

	void ImportRowMajorData_Orders(char **loadedData, int pos);

	void ImportRowMajorData_Stock(char **loadedData, int pos);

	void ImportRowMajorData_Warehouse(char **loadedData, int pos);

};

