#install.packages("tictoc");
#install.packages("RMySQL");
# can be helpful if you got an error with installing postgreSQL
# sudo apt-get install libpq-dev
library(tictoc);
library(RMySQL)

source("config.R");
   
conn = dbConnect(MySQL(), user = username, password = password,dbname = dbname, host = hostname);
   dbGetQuery(conn,paste("SET max_heap_table_size =", table_max_length_bytes , ";", sep = " "));


for(sf in seq(from = start_scale_factor, to = end_scale_scale_factor, by = scale_factor_increment)) {
   
   current_scale_factor = sf;
   source("sql_queries.R");
   throughtput_container =c(); 	
 
    dbGetQuery(conn,query_create_region);
    dbGetQuery(conn,query_create_supplier);
    dbGetQuery(conn,query_create_partsupp);
    dbGetQuery(conn,query_create_part);
    dbGetQuery(conn,query_create_orders);
    dbGetQuery(conn,query_create_nation);
    dbGetQuery(conn,query_create_lineitem);
    dbGetQuery(conn,query_create_customer);
    
    dbGetQuery(conn,query_load_lineitem);
    dbGetQuery(conn,query_load_region);
    dbGetQuery(conn,query_load_supplier);
    dbGetQuery(conn,query_load_partsupp);
    dbGetQuery(conn,query_load_part);
    dbGetQuery(conn,query_load_orders);
    dbGetQuery(conn,query_load_nation);
    dbGetQuery(conn,query_load_customer);
     
   
    l_shipdate_size = 3;
   
    l_discount_size = 8;
   
    l_quantity = 8;
   
    l_extendedprice = 8;
    
    count_line_item = dbGetQuery(conn,"select count(*) from lineitem");
    total_data_processed = count_line_item * (l_shipdate_size + l_quantity + l_extendedprice + 				l_discount_size);
    total_data_processed = total_data_processed / (1000 * 1000);
    
    for(i in 1:total_num_iteration) {
 
    	tic()
     	dbGetQuery(conn,query_tpch_6);
     	exectime <- toc()
     	exectime <- exectime$toc - exectime$tic
     	curr_throughput = total_data_processed / exectime;
     	throughtput_container =  c(as.numeric(curr_throughput),throughtput_container);
     	csv_line =paste(sf, i ,total_data_processed, exectime, curr_throughput,
                	     sep = ",", collapse = "\n");
     		
    	write(csv_line , file = path_to_log_file, append = TRUE);
    }  

    dbGetQuery(conn,query_drop_customer);
    dbGetQuery(conn,query_drop_lineitem);
    dbGetQuery(conn,query_drop_nation);
    dbGetQuery(conn,query_drop_orders);
    dbGetQuery(conn,query_drop_part);
    dbGetQuery(conn,query_drop_region);
    dbGetQuery(conn,query_drop_partsupp);
    dbGetQuery(conn,query_drop_supplier);

    result_line = paste(sf, median(throughtput_container),
                        sep = ",", collapse = "\n");
   write(result_line , file = path_to_results, append = TRUE);
}

