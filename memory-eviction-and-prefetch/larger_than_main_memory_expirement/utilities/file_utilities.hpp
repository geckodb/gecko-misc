//
// Created by Mahmoud Mohsen on 6/1/17.
//
#pragma once

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <time.h>
#include <string>
#include "../config.hpp"
#include <fstream>

using namespace std;
namespace tpch_utilities {
    namespace file_utilities {

      //  based on https://stackoverflow.com/questions/2393345/how-to-append-text-to-a-text-file-in-c
        static void appendLineToFile(string filepath, string line)
      {
          std::ofstream file;
          //can't enable exception now because of gcc bug that raises ios_base::failure with useless message
          //file.exceptions(file.exceptions() | std::ios::failbit);
          file.open(filepath, std::ios::out | std::ios::app);
          if (file.fail())
              throw std::ios_base::failure(std::strerror(errno));

          //make sure write fails with exception if something is wrong
          file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

          file << line << std::endl;
      }

      static void append_double_to_file(string filepath , double field, bool last_in_line = false)
        {
            std::ofstream file;
            //can't enable exception now because of gcc bug that raises ios_base::failure with useless message
            //file.exceptions(file.exceptions() | std::ios::failbit);
            file.open(filepath, std::ios::out | std::ios::app);
            if (file.fail())
                throw std::ios_base::failure(std::strerror(errno));

            //make sure write fails with exception if something is wrong
            file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

            char sep = (last_in_line)? '\n' : ',';
            file << field << sep;
        }
    }
}
