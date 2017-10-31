#!/bin/bash

path_to_dbgen="/home/mohsen/tpch-dbgen"
start_sf=7
end_sf=7
sf_incr=2
total_num_itr=1
ex_swapping_sf=7

for   (( sf=$start_sf; sf <= $end_sf; sf = sf + $sf_incr  ))
	do
	echo "creating schema"
        sqlcmd<<EOFVOLT
	FILE ./sql/create_schema.sql;
EOFVOLT

	if [ $sf -ge $ex_swapping_sf ]
		then
		total_num_itr=8 
	fi
	
	echo "loading tables"

	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/lineitem.csv" lineitem  
	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/orders.csv"   orders  
	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/customer.csv" customer  
	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/partsupp.csv" partsupp  
	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/supplier.csv" supplier  
	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/nation.csv"   nation  
	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/region.csv"   region  
	csvloader  --separator "|"  --file "$path_to_dbgen/$sf/part.csv"     part  
	
	echo "measuring how much data is processed"

		sqlcmd >output2.txt <<EOFVOLT
		FILE ./sql/count_lines.sql;
EOFVOLT
	
	l_shipdate_size=8
	l_discount_size=8   
	l_quantity=8   
	l_extendedprice=8   
	count_line_item=$( grep "[0-9]\+$" output2.txt)
	echo $count_line_item

	total_data_per_row=$(($l_shipdate_size + $l_quantity + $l_extendedprice + $l_discount_size))
	mb=$((1000*1000))

	total_data_processed=$( echo "$total_data_per_row * $count_line_item / $mb" | bc -l )
	echo $total_data_processed

	for (( itr=1; itr <= $total_num_itr  ; ++itr  )) 

		do

		echo "executing the query number 6 of the tpch-h benchmark"

		sqlcmd --query-timeout=2147483647 >output.txt <<EOFVOLT
		FILE ./sql/query_6.sql;
EOFVOLT

		time_taken=$( grep "[0-9]\+\.[0-9]\+s" output.txt |  grep -oh "[0-9]\+\.[0-9]\+" )

		echo $time_taken

		throughput=$( echo "$total_data_processed / $time_taken" | bc -l )

		echo $throughput

		echo "deleting tables"


		echo "$sf,$itr,$time_taken,$total_data_processed,$throughput" >> voltdb_logs.csv	

		done

	echo "$sf,$throughput" >> voltdb_results.csv
	
	sqlcmd<<EOFVOLT
	      FILE ./sql/drop_tables.sql;
EOFVOLT
done
