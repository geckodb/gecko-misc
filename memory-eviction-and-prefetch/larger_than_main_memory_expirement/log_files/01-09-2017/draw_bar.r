library(plotly)

monet_db = read.csv(file = "monetdblogs.csv", header =  F);
monet_db$V4= sort(monet_db$V4)
mysql_db = read.csv(file = "std_bfr_mysql_log.csv", header =  F);
mm_mysql_db = read.csv(file = "mmysql_log.csv", header =  F);
#cpp = read.csv(file = "prefetch_logs.csv", header =  F);


scale_factors <- mm_mysql_db$V1;
data <- data.frame(scale_factors, monet_db$V4,mm_mysql_db$V4,mysql_db$V4)

plot_ly(data, x = ~scale_factors, y = ~monet_db.V4, type = 'bar', name = 'monetdb') %>%
#  add_trace(y = ~cpp.V4, name = 'lazy loading(c++)') %>%
    add_trace(y = ~mm_mysql_db.V4, name = 'mysql(Memory)') %>%
  add_trace(y = ~mysql_db.V4, name = 'mysql(Innodb)') %>%
    layout(xaxis=list(title =("scale factor")),yaxis = list(title = 'Latency (seconds)'), barmode = 'group')


