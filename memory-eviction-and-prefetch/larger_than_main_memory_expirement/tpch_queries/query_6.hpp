//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <schema_include.hpp>
#include <date_utilities.hpp>

/**
 * Query 6
 * -- TPC-H Query 6
 * select sum(l_extendedprice * l_discount) as revenue
 * from
 *      lineitem
 *  where
 *      l_shipdate >= date '1994-01-01'
 *      and l_shipdate < date '1995-01-01'
 *      and l_discount between 0.06 - 0.01 and 0.06 + 0.01
 *      and l_quantity < 24
 */



namespace queries{

    class query6 {

        private:
            tpch_schema::line_item *m_l_item;


    public:
            query6 (tpch_schema::line_item *l_item):m_l_item(l_item)
            {

            }

        long double execute_query6 ()
        {
            long double revenue = 0;
            size_t num_elements = m_l_item->comment.size();
            auto ship_date_data = m_l_item->ship_date.data();
            auto discount_data = m_l_item->discount.data();
            auto quantity_data = m_l_item->quantity.data();
            auto extended_price_data = m_l_item->extended_price.data();

            for (size_t i = 0; i < num_elements ; ++i) {

                if (*(ship_date_data + i) >= tpch_utilities::date_utilities::get_unix_time_stamp("1994-01-01")
                    && *(ship_date_data + i) < tpch_utilities::date_utilities::get_unix_time_stamp("1995-01-01")
                    && *(discount_data + i) >= 0.05
                    && *(discount_data + i) <= 0.07
                    && *(quantity_data + i) < 24) {
                    revenue +=   *(extended_price_data + i) * *(discount_data + i);
                }
            }

            return revenue;
        }


        unsigned long long get_data_processed_size ()
            {
                return m_l_item->extended_price.size() * sizeof(double)
                       + m_l_item->discount.size() * sizeof(double)
                       + m_l_item->ship_date.size() * sizeof(int)
                       + m_l_item->quantity.size() * sizeof(int);

            }

    };


}