/**
 *[this is implemented to work on linux]
 *
 *the code here is taken from the following post
 *
 * https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
 */
#pragma once


#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <stdio.h>

namespace tpch_utilities {
    namespace memory_utilities {

        int parseLine(char* line){
            // This assumes that a digit will be found and the line ends in " Kb".
            int i = strlen(line);
            const char* p = line;
            while (*p <'0' || *p > '9') p++;
            line[i-3] = '\0';
            i = atoi(p);
            return i;
        }

        long long get_total_physical_memory() {
            struct sysinfo memInfo;
            sysinfo (&memInfo);
            long long totalPhysMem = memInfo.totalram;
            //Multiply in next statement to avoid int overflow on right hand side...
            totalPhysMem *= memInfo.mem_unit;
            return totalPhysMem;
        }


        long long get_used_physical_memory() {
            struct sysinfo memInfo;
            sysinfo (&memInfo);
            FILE* file = fopen("/proc/meminfo", "r");
            long long result = -1;
            char line[128];

            while (fgets(line, 128, file) != NULL){
                if (strncmp(line, "MemAvailable:", 13) == 0){
                    result = parseLine(line);
                    break;
                }
            }
            fclose(file);
            return get_total_physical_memory() - (result * 1000);
        }

    }
}
