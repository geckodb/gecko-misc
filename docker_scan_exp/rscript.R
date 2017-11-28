
require(ggplot2)
library("scales")

#reverselog_trans <- function(base = exp(1)) {
#  trans <- function(x) -log(x, base)
#  inv <- function(x) base^(-x)
#  trans_new(paste0("reverselog-", format(base)), trans, inv, 
#            log_breaks(base = base), 
#            domain = c(1e-100, Inf))
#}

reverselog_trans <- function() {
  base = 2
  trans <- function(x) log(x, base)
  inv <- function(x) '^'(base, x)
  trans_new(paste0("reverselog-", format(base)), trans, inv, 
                        log_breaks(base = base), 
                        domain = c(1e-100, Inf))
}

docker_17er <- read.csv(file="/Users/marcus/git/gecko/gecko-misc/docker_scan_exp/results/results_docker_17er.csv",head=TRUE,sep=";")
#native_17er <- read.csv(file="/Users/marcus/git/gecko/gecko-misc/docker_scan_exp/results/results_plain_17er.csv",head=TRUE,sep=";")

#docker_mac <- read.csv(file="/Users/marcus/git/gecko/gecko-misc/docker_scan_exp/results/results_docker_mac.csv",head=TRUE,sep=";")
native_17er <- read.csv(file="/Users/marcus/git/gecko/gecko-misc/docker_scan_exp/results/results_plain_17er.csv",head=TRUE,sep=";")


mean_up = function (data_source) {
  data_source = subset(data_source, data_source$num_requests == 2048)
  agg_data = aggregate(data_source$job_full_wallclock_time_msec, list(data_source$num_threads), mean)  
  agg_data$Group.1 = sapply(agg_data$Group.1, function(x) { return(log(x, 2)) })
  return(agg_data)
}

agg_native = mean_up(native_17er)
agg_docker = mean_up(docker_17er)

agg_data = data.frame(
  x = agg_native$Group.1,
  y_native = sapply(agg_native$x, function(x) { x / 2048 }), 
  y_docker = sapply(agg_docker$x, function(x) { x / 2048 })
)


plot(x = agg_data$x, y = agg_data$Group.1, type="l")

ggplot(agg_data, aes(
  x=(agg_data$x))) +
  geom_line(aes(y = agg_data$y_native, colour = "native")) + 
  geom_line(aes(y = agg_data$y_docker, colour = "docker")) + 
  #scale_y_continuous(limits = c(1.6, 4.2), expand = c(0,0))+
  #scale_x_continuous(limits = c(0,1024)) + 
  scale_x_continuous(breaks=agg_data$x, labels=sapply(agg_data$x, function(x) { return('^'(2, x)) })) + 
  xlab("number threads spawned") + 
  ylab("request latency [msec/request]") + 
  ggtitle("Overhead Application Dockerization (2048 identical requests with selectivity of 50% on 4.13 GiB column)") +
  theme_bw()  + theme(
    plot.title = element_text(size=10),
    axis.title.x = element_text(size=10),
    axis.title.y = element_text(size=10)
  ) 