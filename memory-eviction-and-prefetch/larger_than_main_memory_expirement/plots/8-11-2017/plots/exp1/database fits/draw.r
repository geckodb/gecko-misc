library(plotly)

memory_csv=read.csv(file = "mem.csv", header =  F);
inno_csv=read.csv(file = "inno.csv", header =  F);

configurations <- c("bfr=15%", "bfr=30%", "bfr=45%","bfr=60%","bfr=75%","bfr=90%","memory")
latency <- c(inno_csv$V2[1],inno_csv$V2[2],inno_csv$V2[3],inno_csv$V2[4],inno_csv$V2[5],inno_csv$V2[6],memory_csv$V2)
throughput <- c(inno_csv$V3[1],inno_csv$V3[2],inno_csv$V3[3],inno_csv$V3[4],inno_csv$V3[5],inno_csv$V3[6],memory_csv$V3)
data <- data.frame(configurations, latency, throughput);
m <- list(
  l = 50,
  r = 200,
  b = 100,
  t = 0,
  pad = 1
)
plot_ly(data, x = ~configurations, y = ~latency, type = 'bar', name = 'Latency in seconds') %>%
  add_trace(y = ~throughput, name = 'throughput as QPH') %>%
  layout(autosize = F, width = 750, height = 350,margin=m,yaxis = list(title = 'Count'), barmode = 'group')
