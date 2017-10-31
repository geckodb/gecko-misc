//
// Created by Mahmoud Mohsen on 6/1/17.
//
#pragma once

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <time.h>

using namespace std;
namespace tpch_utilities {
    namespace date_utilities {

        int get_unix_time_stamp(std::string date){

            static struct tm time;
            strptime(date.c_str(), "%Y-%m-%d", &time);
            time_t gmttime = timegm(&time);  // timestamp in GMT
            return gmttime;
        }
    }
}
