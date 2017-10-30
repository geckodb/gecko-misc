
#include"Row_Store_Data.h"
#include"Column_Store_Data.h"
#include<iostream>
#include <cstring>

using namespace std;
class QueryDataGenerator
{

public:
	QueryDataGenerator();
	~QueryDataGenerator();
	 std::string GenerateRandomString(int length);
	 std::string CurrentDateTime();
	 int GenerateRandomNumber(int min, int max);
	 char* GenerateRowStoreInsertData(int numberOfQueries);
	 InputDataInsertColumnStore GenerateColumnStoreInsertData(int numberOfQueries, ColumnStoreData columnStoreData);
	 int* GenerateIndexes(int numberOfQueries);
     InputDataNewOrderColumnStore GenerateColumnStoreNewOrderData(int NumberOfQueries, ColumnStoreData columnStoreData);
     InputDataNewOrderRowStore GenerateRowStoreNewOrderData(int queryCount, char** loadedData);
};



