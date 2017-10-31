set.seed(1) # just to get the same random numbers
par(xpd=FALSE) # this is usually the default

mm_mysql_db = read.csv(file = "mmysql_log_with_swaping_results.csv", header =  F);
volt_db = read.csv(file = "voltdb_swapping.csv", header =  F);

plot(mm_mysql_db$V2, mm_mysql_db$V5, type="o", col="black", pch=2, lty=2, bty='L',  ylim=c(0,350), ylab="throughput(MB/Seconds)" , xlab ="Iteration" )

points(volt_db$V2, volt_db$V5,  col="orange", pch=1);
lines(volt_db$V2, volt_db$V5,  col="orange",lty=1);

points(mm_mysql_db$V1, mm_mysql_db$V5,  col="black", pch=20);
lines(mm_mysql_db$V1, mm_mysql_db$V5,  col="black",lty=4);

par(xpd=TRUE)
legend(1.5, 450, legend=c("mysql(memory)", "Voltdb"),
       col=c("black", "orange"), pch =c(2,1) , lty=c(2,1) , cex=0.75, horiz = T)
