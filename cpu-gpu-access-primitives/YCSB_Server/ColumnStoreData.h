#ifndef COLUMNSTOREDATA_H_
#define COLUMNSTOREDATA_H_

#include <vector>


struct charArray100 {
    char array[100];
};

struct charArray255 {
    char array[255];
};


struct UserCS {
std::vector<charArray255> KEY;
std::vector<charArray100> FIELD0;
std::vector<charArray100> FIELD1;
std::vector<charArray100> FIELD2;
std::vector<charArray100> FIELD3;
std::vector<charArray100> FIELD4;
std::vector<charArray100> FIELD5;
std::vector<charArray100> FIELD6;
std::vector<charArray100> FIELD7;
std::vector<charArray100> FIELD8;
std::vector<charArray100> FIELD9;
};

#endif
