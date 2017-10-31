dbname ="tpch2";

username ="root";

password ="mysql";

hostname ="127.0.0.1";

path_to_tables = "/home/mohsen/tpch-dbgen/";

path_to_q6_tiny_log_file = "/home/mohsen/expirement_results/small_second_inno_log_results6_tiny.csv";

path_to_q6_tiny_results = "/home/mohsen/expirement_results/small_second_inno_results6_tiny.csv";

path_to_q6_log_file = "/home/mohsen/expirement_results/small_second_inno_log_results6.csv";

path_to_q6_results = "/home/mohsen/expirement_results/small_second_inno_results6.csv";


start_scale_factor = 1;

end_scale_scale_factor = 1;

scale_factor_increment = 1;

query_repetition = 5;

max_page_size = 6 * 1024 * 1024 * 1024;

buffer_pool_memory_percentage =c( 0.15, 0.3 , 0.45 , 0.6 , 0.75 , 0.9 ) ;
#buffer_pool_memory_percentage =c( 0.15,0.9 ) ;

default_buffer_pool_size = 134217728



