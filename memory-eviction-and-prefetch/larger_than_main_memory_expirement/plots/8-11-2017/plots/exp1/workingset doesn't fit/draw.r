library(plotly)

memory_csv=read.csv(file = "mem.csv", header =  F);
inno_csv=read.csv(file = "inno.csv", header =  F);

configurations <- c("bfr=0.15", "bfr=0.30", "bfr=0.45","bfr=0.60","bfr=0.75","bfr=0.90","bfr=1.0","memory")
latency <- c(inno_csv$V2[1]/(60*60),inno_csv$V2[2]/(60*60),inno_csv$V2[3]/(60*60),inno_csv$V2[4]/(60*60),inno_csv$V2[5]/(60*60),inno_csv$V2[6]/(60*60),inno_csv$V2[7]/(60*60),memory_csv$V2/(60*60))
throughput <- c(inno_csv$V3[1]*(60),inno_csv$V3[2]*(60),inno_csv$V3[3]*(60),inno_csv$V3[4]*(60),inno_csv$V3[5]*(60),inno_csv$V3[6]*(60),inno_csv$V3[7]*(60),memory_csv$V3*(60))
data <- data.frame(configurations, latency, throughput);
m <- list(
  l = 50,
  r = 200,
  b = 100,
  t = 0,
  pad = 1
)
plot_ly(data, x = ~configurations, y = ~latency, type = 'bar', name = 'Latency in Hours') %>%
  add_trace(y = ~throughput, name = 'throughput QP60H') %>%
  layout(autosize = F, width = 750, height = 350,margin=m,yaxis = list(title = 'Count'), barmode = 'group')
