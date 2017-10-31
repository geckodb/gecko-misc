//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <string>
#include <vector>

namespace tpch_schema {

    class region {

    public :
        std::vector <std::string> region_key;
        std::vector <std::string> name;
        std::vector <std::string> comment;

    public:
        region()
        {

        }

        ~region()
        {
            region_key.clear();
            name.clear();
            comment.clear();
        }

    };

}
