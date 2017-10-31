//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <string>
#include <vector>

namespace tpch_schema {

    class orders {

    public :
        std::vector <std::string> order_key;
        std::vector <std::string> custkey;
        std::vector <std::string> order_status;
        std::vector <std::string> total_price;
        std::vector <std::string> order_date;
        std::vector <std::string> order_priority;
        std::vector <std::string> clerk;
        std::vector <std::string> ship_priority;
        std::vector <std::string> comment;

    public:
        orders()
        {

        }

        ~orders()
        {
            order_key.clear();
            custkey.clear();
            order_status.clear();
            total_price.clear();
            order_date.clear();
            order_priority.clear();
            clerk.clear();
            ship_priority.clear();
            comment.clear();
        }

    };

}
