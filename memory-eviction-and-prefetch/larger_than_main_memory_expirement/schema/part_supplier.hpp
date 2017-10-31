//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <string>
#include <vector>

namespace tpch_schema {

    class part_supplier {

    public :
        std::vector <std::string> part_key;
        std::vector <std::string> sup_key;
        std::vector <std::string> availqty;
        std::vector <std::string> supply_cost;
        std::vector <std::string> comment;

    public:
        part_supplier()
        {

        }

        ~part_supplier()
        {
            part_key.clear();
            sup_key.clear();
            availqty.clear();
            supply_cost.clear();
            comment.clear();
        }

    };

}
