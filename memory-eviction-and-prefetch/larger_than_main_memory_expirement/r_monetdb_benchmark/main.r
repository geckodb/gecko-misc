#install.packages("tictoc");
#install.packages("MonetDB.R");
#install.packages("RMySQL");
library(tictoc);
library(MonetDB.R);
library("RMySQL");

source("config.R");

conn <- dbConnect(MonetDB.R(), host=hostname, dbname=dbname, user=username, password=password);

for(sf in seq(from = start_scale_factor, to = end_scale_scale_factor, by = scale_factor_increment)) {
   current_scale_factor = sf;
   source("sql_queries.R");
   throughtput_container =c();
   
   for(i in 1:total_num_iteration) {
     
     dbGetQuery(conn,query_create_tables);
     dbGetQuery(conn,query_load_tables);
     l_shipdate_size = dbGetQuery(conn,"select  count * typewidth   from  sys.storage() where table = 'lineitem' and column ='l_shipdate'; ");
     
     l_discount_size = dbGetQuery(conn,"select  count * typewidth   from  sys.storage() where table = 'lineitem' and column ='l_discount'; ");
     
     l_quantity = dbGetQuery(conn,"select  count * typewidth   from  sys.storage() where table = 'lineitem' and column ='l_quantity'; ");
     
     l_extendedprice = dbGetQuery(conn,"select  count * typewidth   from  sys.storage() where table = 'lineitem' and column ='l_extendedprice'; ");
     
     total_data_processed = l_shipdate_size + l_quantity + l_extendedprice + l_discount_size;
     
     total_data_processed = total_data_processed / (1000 * 1000);
     
     tic()
     dbGetQuery(conn,query_tpch_6);
     exectime <- toc()
     exectime <- exectime$toc - exectime$tic
     curr_throughput = total_data_processed / exectime;
     throughtput_container =  c(as.numeric(curr_throughput),throughtput_container);
     csv_line =paste(sf, i, total_data_processed, exectime, curr_throughput,
                     sep = ",", collapse = "\n");
     
     write(csv_line , file = path_to_log_file, append = TRUE);
     dbGetQuery(conn,query_drop_tables);
     
     }
   
   result_line = paste(sf, median(throughtput_container),
                        sep = ",", collapse = "\n");
   write(result_line , file = path_to_results, append = TRUE);
}
