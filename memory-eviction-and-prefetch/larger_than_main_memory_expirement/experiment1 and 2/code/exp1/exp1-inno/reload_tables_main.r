#install.packages("tictoc");
#install.packages("RMySQL");
# can be helpful if you got an error with installing postgreSQL
# sudo apt-get install libpq-dev
library(tictoc);
library(RMySQL);

source("config.R");


conn = dbConnect(MySQL(), user = username, password = password,dbname = dbname, host = hostname);


    current_scale_factor = start_scale_factor;
    source("sql_queries.R");

for(bf_pool_rel in buffer_pool_memory_percentage  ) {

    cat(paste("bf ratio =",bf_pool_rel,"\n",sep=";") );   
    curr_bf_pool_size = round(bf_pool_rel * max_page_size,0);
    
    set_pool_size_query = paste("SET GLOBAL innodb_buffer_pool_size=",curr_bf_pool_size,";",
                        sep = "");


    dbGetQuery(conn,set_pool_size_query);
    
    Sys.sleep(60 * 5 );
   
     print(dbGetQuery(conn,"SELECT @@innodb_buffer_pool_size;"));

    
    cat("creating tables \n");
    dbGetQuery(conn,query_create_region);
    dbGetQuery(conn,query_create_supplier);
    dbGetQuery(conn,query_create_partsupp);
    dbGetQuery(conn,query_create_part);
    dbGetQuery(conn,query_create_orders);
    dbGetQuery(conn,query_create_nation);
    dbGetQuery(conn,query_create_lineitem);
    dbGetQuery(conn,query_create_customer);
    dbGetQuery(conn,query_create_lineitem_tiny);
    
    cat("loading tables \n");    
    dbGetQuery(conn,query_load_lineitem);
    dbGetQuery(conn,query_load_lineitem_tiny);
    dbGetQuery(conn,query_load_region);
    dbGetQuery(conn,query_load_supplier);
    dbGetQuery(conn,query_load_partsupp);
    dbGetQuery(conn,query_load_part);
    dbGetQuery(conn,query_load_orders);
    dbGetQuery(conn,query_load_nation);
    dbGetQuery(conn,query_load_customer);

    
    cat("loading the working set to the memory \n");
    dbGetQuery(conn,query_tpch_6_tiny);
    cat("Executing query 6 to the tiny table \n");
    q6_latency = c();
    for(i in 1:query_repetition) {
 
    	tic()
     	dbGetQuery(conn,query_tpch_6_tiny);
     	exectime <- toc()
     	exectime <- exectime$toc - exectime$tic
     	q6_latency =c(exectime,q6_latency);

    }  
    total_latency= sum(q6_latency)
    query_per_hour= (query_repetition * 60) / (total_latency) 
    result_line = paste(bf_pool_rel, median(q6_latency), query_per_hour,
                        sep = ",", collapse = "\n");
    cat("writing the result of query 6 tiny \n");
    write(result_line , file = path_to_q6_tiny_results, append = TRUE);

    cat("loading the working set to the memory \n");
    dbGetQuery(conn,query_tpch_6);

    q6_latency = c();
    cat("Executing query 6 \n");
    for(i in 1:query_repetition) {
 
    	tic()
     	dbGetQuery(conn,query_tpch_6);
     	exectime <- toc()
     	exectime <- exectime$toc - exectime$tic
     	q6_latency =c(exectime,q6_latency);
     	   
 }  
    total_latency= sum(q6_latency)
    query_per_hour= (query_repetition * 60) / (total_latency) 
    result_line = paste(bf_pool_rel, median(q6_latency), query_per_hour,
                        sep = ",", collapse = "\n");
    cat("writing the result of query 6 \n");
    write(result_line , file = path_to_q6_results, append = TRUE);

    cat("deleting tables \n");
    dbGetQuery(conn,query_drop_customer);
    dbGetQuery(conn,query_drop_lineitem);
    dbGetQuery(conn,query_drop_nation);
    dbGetQuery(conn,query_drop_orders);
    dbGetQuery(conn,query_drop_part);
    dbGetQuery(conn,query_drop_region);
    dbGetQuery(conn,query_drop_partsupp);
    dbGetQuery(conn,query_drop_supplier);
    dbGetQuery(conn,query_drop_lineitem_tiny);
    		
}

  
    system("sudo /etc/init.d/mysql restart");
    
  
