#Experiment 2

what we are doing here. is to the YCSB benchmark with different workload skew between the read and the write.

and to use:

MYSQL: with memory engine

MYSQL: with innodb and with different bufferpool variations, but notice here that the mysql files is placed into main-memory by forms of mounting a file into the main memory.

## Configuration used

the datasource is the ycsb, we have used this framework:

https://github.com/oltpbenchmark/oltpbench

please note we have to mention this if we used it in a paper.

the configurations for the ycsb is:

	scale factor = 5000
	number of terminals = 10
        time = 240
	

