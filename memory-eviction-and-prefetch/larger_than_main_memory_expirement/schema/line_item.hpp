//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <string>
#include <vector>

namespace tpch_schema {

    struct var_char {
        char *char_arr;
    };


    class line_item {

        public :
            std::vector <int> order_key;
            std::vector <int> part_key;
            std::vector <int> supp_key;
            std::vector <int> line_number;
            std::vector <int> quantity;
            std::vector <double> extended_price;
            std::vector <double> discount;
            std::vector <double> tax;
            std::vector <int> ship_date;
            std::vector <int> commit_date;
            std::vector <int> receipt_date;
            std::vector <char> return_flag;
            std::vector <char> line_status;
            std::vector <var_char>  ship_instruct;
            std::vector <var_char> ship_mode;
            std::vector <std::string> comment;

        public:
            line_item()
            {

            }
            unsigned long long get_table_size() {
                unsigned long long result = 0;
                result += receipt_date.size() * 8 * sizeof(int);
                result += return_flag.size() * 2 * sizeof(char);
                result += discount.size() * 3 * sizeof(double);
                result += comment.size() * 1 * sizeof(std::string);
                result += ship_instruct.size() * 1 * sizeof(char) * 11;
                result += ship_mode.size() * 1 * sizeof(char) * 4;

                return result;
            }

        unsigned long long get_table_count() {

            return ship_mode.size();
        }
        ~line_item()
        {
            order_key.clear();
            part_key.clear();
            supp_key.clear();
            line_number.clear();
            quantity.clear();
            extended_price.clear();
            discount.clear();
            tax.clear();
            ship_date.clear();
            commit_date.clear();
            receipt_date.clear();
            return_flag.clear();
            line_status.clear();
            ship_instruct.clear();
            ship_mode.clear();
            comment.clear();
        }

    };

}
