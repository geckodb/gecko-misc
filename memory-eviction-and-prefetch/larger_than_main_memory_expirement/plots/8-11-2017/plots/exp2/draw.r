library(plotly)

memory_csv=read.csv(file = "mem.csv", header =  F);
inno_25_csv=read.csv(file = "inno_25.csv", header =  F);
inno_50_csv=read.csv(file = "inno_50.csv", header =  F);
inno_75_csv=read.csv(file = "inno_75.csv", header =  F);
inno_100_csv=read.csv(file = "inno_100.csv", header =  F);


configurations <- c("bfr=25%", "bfr=50%", "bfr=75%"," bfr=100%","memory")
r_0 <- c(inno_25_csv$V2[1],inno_50_csv$V2[1],inno_75_csv$V2[1],inno_100_csv$V2[1],memory_csv$V2[1])
r_25 <- c(inno_25_csv$V2[2],inno_50_csv$V2[2],inno_75_csv$V2[2],inno_100_csv$V2[2],memory_csv$V2[2])
r_50 <- c(inno_25_csv$V2[3],inno_50_csv$V2[3],inno_75_csv$V2[3],inno_100_csv$V2[3],memory_csv$V2[3])
r_75 <- c(inno_25_csv$V2[4],inno_50_csv$V2[4],inno_75_csv$V2[4],inno_100_csv$V2[4],memory_csv$V2[4])
r_100 <-c(inno_25_csv$V2[5],inno_50_csv$V2[5],inno_75_csv$V2[5],inno_100_csv$V2[5],memory_csv$V2[5])

xform <- list(categoryorder = "array",
              categoryarray = c("bfr=25%", "bfr=50%", "bfr=75%"," bfr=100%","memory"))

data <- data.frame(configurations, r_0, r_25, r_50, r_75, r_100);
m <- list(
  l = 50,
  r = 200,
  b = 100,
  t = 0,
  pad = 1
)
plot_ly(data, x = ~configurations, y = ~r_0, type = 'bar', name = 'read=0%') %>%
  add_trace(y = ~r_25, name = 'read=25%') %>%
  add_trace(y = ~r_50, name = 'read=50%') %>%
  add_trace(y = ~r_75, name = 'read=75%') %>%
  add_trace(y = ~r_100, name = 'read=100%') %>%
  layout(autosize = F, width = 750, height = 350,margin=m,yaxis = list(title = 'Latency in milli seconds'),xaxis = xform, barmode = 'group')
