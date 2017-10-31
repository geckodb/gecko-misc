//
// Created by Mahmoud Mohsen on 6/1/17.
//


#include <iostream>
#include <schema_include.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <file_utilities.hpp>
#include <query_6.hpp>
#include <read_tables.hpp>
#include <memory_measurer.hpp>
#include <profiling.hpp>
#include <string>

int main() {

    for (auto k = START_SCALE_FACTOR; k <= END_SCALE_FACTOR;k += SCALE_FACTOR_INCR) {


        std::ostringstream ss;
        ss << k;
        std::string s(ss.str());
        vector <double> throughput_container;
        input_stream::curr_scaling_factor = s;
        std::cout << "current scaling factor = " << k << std::endl;

        std::cout << "Memory status before reading the tables >>>>>>>>>>>>" << std::endl;

        double used_physical_memory = tpch_utilities::memory_utilities::
                                      get_used_physical_memory() /
                                      ((1.0 * 1000 * 1000));
        std::cout << "used physical memory =" << used_physical_memory << " MB" << std::endl;

        for (auto i = 0; i < NUM_ITERATIONS; ++i) {
            std::cout << "########################################################" << std::endl;

            std::cout << "reading tables >>>>>>>>>>>>" << std::endl;

            std::cout << "reading line item >>>>>>>>>>>>" << std::endl;
            auto l_item = (input_stream::read_line_item());

            std::cout << "reading orders >>>>>>>>>>>>" << std::endl;
            auto ord = (input_stream::read_orders());

            std::cout << "reading region >>>>>>>>>>>>" << std::endl;
            auto reg = input_stream::read_region();

            std::cout << "reading nation >>>>>>>>>>>>" << std::endl;
            auto nat = input_stream::read_nation();

            std::cout << "reading part supplier >>>>>>>>>>>>" << std::endl;
            auto part_supp = input_stream::read_part_supplier();

            std::cout << "reading part customer >>>>>>>>>>>>" << std::endl;
            auto cus = input_stream::read_customer();

            std::cout << "reading supplier >>>>>>>>>>>>" << std::endl;
            auto supp = input_stream::read_supplier();

            std::cout << "reading part >>>>>>>>>>>>" << std::endl;
            auto part = input_stream::read_part();


            std::cout << "########################################################" << std::endl;
            std::cout << "memory status after reading the tables" << std::endl;

            used_physical_memory = tpch_utilities::memory_utilities::
                                   get_used_physical_memory() /
                                   ((1.0 * 1000 * 1000));
            std::cout << "used physical memory =" << used_physical_memory << " MB" << std::endl;

            std::cout << "########################################################" << std::endl;

            unsigned long long current_dur = 0;


            auto q = queries::query6(l_item);

            std::cout << "executing query6>>>>>>>" << std::endl;
            current_dur = utils::profiling::measure<std::chrono::nanoseconds>::execute(
                    [&q]() {
                        std::cout << q.execute_query6() << std::endl;
                    });

            tpch_utilities::file_utilities::append_double_to_file(PATH_TO_LOG_FILE,
                                                                  k);

            tpch_utilities::file_utilities::append_double_to_file(PATH_TO_LOG_FILE,
                                                                  i + 1);

            std::cout << "memory status after Query" << std::endl;

            used_physical_memory = tpch_utilities::memory_utilities::
                                   get_used_physical_memory() /
                                   ((1.0 * 1000 * 1000));

            std::cout << "used physical memory =" << used_physical_memory << " MB" << std::endl;

            tpch_utilities::file_utilities::append_double_to_file(PATH_TO_LOG_FILE,
                                                                  used_physical_memory);


            double process_time = (1.0 * current_dur) / (1.0 * 1000000000);
            std::cout << "time =" << process_time << "seconds" << std::endl;

            tpch_utilities::file_utilities::append_double_to_file(PATH_TO_LOG_FILE,
                                                                  process_time);

            long double data_processed = q.get_data_processed_size() / (1.0 * 1000 * 1000);
            std::cout << "data processed =" << data_processed << "MB" << std::endl;

            tpch_utilities::file_utilities::append_double_to_file(PATH_TO_LOG_FILE,
                                                                  data_processed);

            double throughput = data_processed / process_time;
            throughput_container.push_back (throughput);
            std::cout << "throughput =" << throughput << "MB/sec" << std::endl;

            tpch_utilities::file_utilities::append_double_to_file(PATH_TO_LOG_FILE,
                                                                  throughput, true);
            delete (l_item);
            delete (cus);
            delete (ord);
            delete (nat);
            delete (reg);
            delete (supp);
            delete (part_supp);
            delete (part);

            usleep(1 * 1000000);
        }
        used_physical_memory = tpch_utilities::memory_utilities::
                               get_used_physical_memory()/
                               ((1.0 * 1000 * 1000));
        std::cout<<"used physical memory ="<<used_physical_memory<<" MB"<<std::endl;


        tpch_utilities::file_utilities::append_double_to_file(PATH_TO_RESULT,
                                                              k);

        sort(throughput_container.begin(),throughput_container.end());
        int median_index = ((NUM_ITERATIONS + 1) /2) - 1;
        tpch_utilities::file_utilities::append_double_to_file(PATH_TO_RESULT,
                                                              throughput_container.at(median_index), true);

    }

    return 0;
}