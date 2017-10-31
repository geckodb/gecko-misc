set.seed(1) # just to get the same random numbers
par(xpd=FALSE) # this is usually the default

mm_mysql_db = read.csv(file = "mmysql_log_with_swaping_results.csv", header =  F);

plot(mm_mysql_db$V2, mm_mysql_db$V4, type="o", col="black", pch=2, lty=2, bty='L', ylab="Latency(seconds)" , xlab ="Iteration" )


par(xpd=TRUE)
legend(3.5, 7450, legend=c("mysql(memory)"),
       col=c("black"), pch =c(2) , lty=c(2) , cex=0.75, horiz = T)
