//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <string>
#include <vector>

namespace tpch_schema {

    class nation {

    public :
        std::vector <std::string> nation_key;
        std::vector <std::string> name;
        std::vector <std::string> region_key;
        std::vector <std::string> comment;

    public:
        nation()
        {

        }

        ~nation()
        {
            nation_key.clear();
            name.clear();
            region_key.clear();
            comment.clear();
        }

    };

}
