# Experiment 1

in this experiment we are measuring the effect on three different settings

1- if the database fits entirly into main memory
2- if the database doesn't fit into main memory but still the working set fits.
3- if the working set doesn't fit into main memory.


we have used Mysql with memory engine to represent a pure in-memory DBMS.
we have used Mysql with Innodb to represent a buffer pool DBMS.

## settings

The source of the data is the tpch benchmark, we are using query 6 .... the query is executed 5 times to estimate the throughtput.

for the working set fits but the databse doesn't we have used the lineitem table
with a data size of 60 G.

for the working set doesn't fit we have used the lineitem table with table size above 1.5 T.

The experiment is done on a server with 256 G RAM.

## expected outcome

what we have expected here:

for in-memory database if the working set fits the performance won't be too much badly effected even if the database doesn't fit. as with pagging the working set would be loaded entirly in the main-memory

for inno-db with increasing the puffer poo the performance will be near to a pure in-memory database. 


## The format of the logfiles

there are two types of log files

1- logfiles
2- results

the difference is the results ones, are the plot ready ... log are more detailed

the logfiles for the inno db part follows the following format

	bufferpool relative size, the query iteration, the latency in seconds

then the result file aggregates the results by summing all the latencies we have got

	total_latency = sum (measured_latencies)

then

	throughput = num_queries / total_latency

for memory it is similar except that we don't consider the bufferpool size



