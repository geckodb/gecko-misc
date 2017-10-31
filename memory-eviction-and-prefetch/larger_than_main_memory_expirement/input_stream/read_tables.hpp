//
// Created by Mahmoud Mohsen on 6/1/17.
//

#pragma once

#include <fstream>
#include <string>
#include <schema_include.hpp>
#include "../config.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <date_utilities.hpp>
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include <line_item.hpp>
using namespace std;

namespace input_stream {

    string curr_scaling_factor;
    tpch_schema::line_item *read_line_item () {
        auto l_itm = new tpch_schema::line_item();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(LINE_ITEM_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return l_itm;
        }
        string s_order_key;
        string s_part_key;
        string s_supp_key;
        string s_line_number;
        string s_quantity;
        string s_extended_price;
        string s_discount;
        string s_tax;
        string s_return_flag;
        string s_line_status;
        string s_ship_date;
        string s_commit_date;
        string s_receipt_date;
        string s_ship_instruct;
        string s_ship_mode;
        string s_comment;

        while ( getline(in,s_order_key,'|')) {
            l_itm->order_key.push_back(lexical_cast<int>(s_order_key));

            getline(in,s_part_key,'|');
            l_itm->part_key.push_back(lexical_cast<int>(s_part_key));

            getline(in,s_supp_key,'|');
            l_itm->supp_key.push_back(lexical_cast<int>(s_supp_key));

            getline(in,s_line_number,'|');
            l_itm->line_number.push_back(lexical_cast<int>(s_line_number));

            getline(in,s_quantity,'|');
            l_itm->quantity.push_back(lexical_cast<int>(s_quantity));

            getline(in,s_extended_price,'|');
            l_itm->extended_price.push_back(lexical_cast<double >(s_extended_price));

            getline(in,s_discount,'|');
            l_itm->discount.push_back(lexical_cast<double>(s_discount));

            getline(in,s_tax,'|');
            l_itm->tax.push_back(lexical_cast<double >(s_tax));

            getline(in,s_return_flag,'|');
            l_itm->return_flag.push_back(lexical_cast<char>(s_return_flag));

            getline(in,s_line_status,'|');
            l_itm->line_status.push_back(lexical_cast<char>(s_line_status));

            getline(in,s_ship_date,'|');
            l_itm->ship_date.push_back(tpch_utilities::date_utilities::get_unix_time_stamp(s_ship_date));

            getline(in,s_commit_date,'|');
            l_itm->commit_date.push_back(tpch_utilities::date_utilities::get_unix_time_stamp(s_commit_date));

            getline(in,s_receipt_date,'|');
            l_itm->receipt_date.push_back(tpch_utilities::date_utilities::get_unix_time_stamp(s_receipt_date));

            getline(in,s_ship_instruct,'|');
            tpch_schema::var_char v1;
            v1.char_arr = const_cast<char*>(s_ship_instruct.c_str());
            l_itm->ship_instruct.push_back(v1);

            getline(in,s_ship_mode,'|');
            tpch_schema::var_char v2;
            v2.char_arr = const_cast<char*>(s_ship_mode.c_str());

            l_itm->ship_mode.push_back(v2);

            getline(in,s_comment,'|');
            l_itm->comment.push_back(s_comment);

            in.ignore();
        }

        in.close();
        return l_itm;
    }


    tpch_schema::orders *read_orders () {
        auto order_t = new tpch_schema::orders();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(ORDERS_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return order_t;
        }
        std::string order_key;
        std::string custkey;
        std::string order_status;
        std::string total_price;
        std::string order_date;
        std::string order_priority;
        std::string clerk;
        std::string ship_priority;
        std::string comment;

        while ( getline(in,order_key,'|')) {
            order_t->order_key.push_back(order_key);

            getline(in,custkey,'|');
            order_t->custkey.push_back(custkey);

            getline(in,order_status,'|');
            order_t->order_status.push_back(order_status);

            getline(in,total_price,'|');
            order_t->total_price.push_back(total_price);

            getline(in,order_date,'|');
            order_t->order_date.push_back(order_date);

            getline(in,order_priority,'|');
            order_t->order_priority.push_back(order_priority);

            getline(in,clerk,'|');
            order_t->clerk.push_back(clerk);

            getline(in,ship_priority,'|');
            order_t->ship_priority.push_back(ship_priority);

            getline(in,comment,'|');
            order_t->comment.push_back(comment);

            in.ignore();
        }

        in.close();
        return order_t;
    }

    tpch_schema::part *read_part () {
        auto part_t = new tpch_schema::part();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(PART_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return part_t;
        }

        std::string part_key;
        std::string name;
        std::string mfgr;
        std::string brand;
        std::string type;
        std::string size;
        std::string container;
        std::string retail_price;
        std::string comment;

        while ( getline(in,part_key,'|')) {
            part_t->part_key.push_back(part_key);

            getline(in,name,'|');
            part_t->name.push_back(name);

            getline(in,mfgr,'|');
            part_t->mfgr.push_back(mfgr);

            getline(in,brand,'|');
            part_t->brand.push_back(brand);

            getline(in,type,'|');
            part_t->type.push_back(type);

            getline(in,size,'|');
            part_t->size.push_back(size);

            getline(in,container,'|');
            part_t->container.push_back(container);

            getline(in,retail_price,'|');
            part_t->retail_price.push_back(retail_price);

            getline(in,comment,'|');
            part_t->comment.push_back(comment);

            in.ignore();
        }

        in.close();
        return part_t;
    }


    tpch_schema::customer *read_customer () {
        auto customer_t = new tpch_schema::customer();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(CUSTOMER_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return customer_t;
        }

        std::string cust_key;
        std::string name;
        std::string address;
        std::string nation_key;
        std::string phone;
        std::string acctbal;
        std::string mktsegment;
        std::string comment;


        while ( getline(in,cust_key,'|')) {
            customer_t->cust_key.push_back(cust_key);

            getline(in,name,'|');
            customer_t->name.push_back(name);

            getline(in,address,'|');
            customer_t->address.push_back(address);

            getline(in,nation_key,'|');
            customer_t->nation_key.push_back(nation_key);

            getline(in,phone,'|');
            customer_t->phone.push_back(phone);

            getline(in,acctbal,'|');
            customer_t->acctbal.push_back(acctbal);

            getline(in,mktsegment,'|');
            customer_t->mktsegment.push_back(mktsegment);

            getline(in,comment,'|');
            customer_t->comment.push_back(comment);

            in.ignore();
        }

        in.close();
        return customer_t;
    }

    tpch_schema::supplier *read_supplier () {
        auto supplier_t = new tpch_schema::supplier();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(SUPPLIER_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return supplier_t;
        }

        std::string sup_key;
        std::string name;
        std::string address;
        std::string nation_key;
        std::string phone;
        std::string acctbal;
        std::string comment;


        while ( getline(in,sup_key,'|')) {
            supplier_t->sup_key.push_back(sup_key);

            getline(in,name,'|');
            supplier_t->name.push_back(name);

            getline(in,address,'|');
            supplier_t->address.push_back(address);

            getline(in,nation_key,'|');
            supplier_t->nation_key.push_back(nation_key);

            getline(in,phone,'|');
            supplier_t->phone.push_back(phone);

            getline(in,acctbal,'|');
            supplier_t->acctbal.push_back(acctbal);

            getline(in,comment,'|');
            supplier_t->comment.push_back(comment);

            in.ignore();
        }

        in.close();
        return supplier_t;
    }

    tpch_schema::part_supplier *read_part_supplier () {
        auto part_supplier_t = new tpch_schema::part_supplier();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(PART_SUPPLIER_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return part_supplier_t;
        }

        std::string part_key;
        std::string sup_key;
        std::string availqty;
        std::string supply_cost;
        std::string comment;


        while ( getline(in,part_key,'|')) {
            part_supplier_t->part_key.push_back(part_key);

            getline(in,sup_key,'|');
            part_supplier_t->sup_key.push_back(sup_key);

            getline(in,availqty,'|');
            part_supplier_t->availqty.push_back(availqty);

            getline(in,supply_cost,'|');
            part_supplier_t->supply_cost.push_back(supply_cost);

            getline(in,comment,'|');
            part_supplier_t->comment.push_back(comment);

            in.ignore();
        }

        in.close();
        return part_supplier_t;
    }

    tpch_schema::nation *read_nation () {

        auto nation_t = new tpch_schema::nation();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(NATION_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return nation_t;
        }

        std::string nation_key;
        std::string name;
        std::string region_key;
        std::string comment;

        while ( getline(in,nation_key,'|')) {
            nation_t->nation_key.push_back(nation_key);

            getline(in,name,'|');
            nation_t->name.push_back(name);

            getline(in,region_key,'|');
            nation_t->region_key.push_back(region_key);

            getline(in,comment,'|');
            nation_t->comment.push_back(comment);

            in.ignore();
        }

        in.close();
        return nation_t;
    }

    tpch_schema::region *read_region () {

        auto region_t = new tpch_schema::region();
        ifstream in(string(PATH_TO_TABLES)+curr_scaling_factor+string("/")+string(REGION_TABLE_NAME));

        if (! in.is_open()) {
            cerr<< "cannot open file";
            return region_t;
        }

        std::string region_key;
        std::string name;
        std::string comment;

        while ( getline(in,region_key,'|')) {
            region_t->region_key.push_back(region_key);

            getline(in,name,'|');
            region_t->name.push_back(name);


            getline(in,comment,'|');
            region_t->comment.push_back(comment);

            in.ignore();
        }

        in.close();
        return region_t;
    }

}