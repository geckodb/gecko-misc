set.seed(1) # just to get the same random numbers
par(xpd=FALSE) # this is usually the default

volt_db = read.csv(file = "voltdb_results.csv", header =  F);
monet_db = read.csv(file = "monetdblogs.csv", header =  F);
mysql_db = read.csv(file = "std_bfr_mysql_log.csv", header =  F);
mm_mysql_db = read.csv(file = "mmysql_log.csv", header =  F);

plot(monet_db$V1, monet_db$V5, type="o", col="blue", pch=2, lty=2, bty='L',  ylim=c(0,300), ylab="throughput(MB/Seconds)" , xlab ="Scaling factor" )

points(volt_db$V1, volt_db$V5,  col="orange", pch=1);
lines(volt_db$V1, volt_db$V5,  col="orange",lty=1);

points(mysql_db$V1, mysql_db$V5,  col="green", pch=15);
lines(mysql_db$V1, mysql_db$V5,  col="green",lty=3);

points(mm_mysql_db$V1, mm_mysql_db$V5,  col="black", pch=20);
lines(mm_mysql_db$V1, mm_mysql_db$V5,  col="black",lty=4);

par(xpd=TRUE)
legend(0.75, 370, legend=c("monetdb", "Voltdb","mysql(InnoDB)","mysql(memory)"),
       col=c("blue", "orange","green","black"), pch =c(2,1,15,20) , lty=c(2,1,3,4) , cex=0.65, horiz = T)
