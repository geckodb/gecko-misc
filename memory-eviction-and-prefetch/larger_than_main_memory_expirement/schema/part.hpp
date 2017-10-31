//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <string>
#include <vector>

namespace tpch_schema {

    class part {

    public :
        std::vector <std::string> part_key;
        std::vector <std::string> name;
        std::vector <std::string> mfgr;
        std::vector <std::string> brand;
        std::vector <std::string> type;
        std::vector <std::string> size;
        std::vector <std::string> container;
        std::vector <std::string> retail_price;
        std::vector <std::string> comment;

    public:
        part()
        {

        }

        ~part()
        {
            part_key.clear();
            name.clear();
            mfgr.clear();
            brand.clear();
            type.clear();
            size.clear();
            container.clear();
            retail_price.clear();
            comment.clear();
        }


    };

}
