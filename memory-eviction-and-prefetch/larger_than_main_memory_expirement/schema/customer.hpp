//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <string>
#include <vector>

namespace tpch_schema {

    class customer {

    public :
        std::vector <std::string> cust_key;
        std::vector <std::string> name;
        std::vector <std::string> address;
        std::vector <std::string> nation_key;
        std::vector <std::string> phone;
        std::vector <std::string> acctbal;
        std::vector <std::string> mktsegment;
        std::vector <std::string> comment;

    public:
        customer()
        {

        }

        ~customer()
        {
            cust_key.clear();
            name.clear();
            address.clear();
            nation_key.clear();
            phone.clear();
            acctbal.clear();
            mktsegment.clear();
            comment.clear();
        }

    };

}
