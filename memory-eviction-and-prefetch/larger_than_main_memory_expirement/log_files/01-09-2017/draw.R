set.seed(1) # just to get the same random numbers
par(xpd=FALSE) # this is usually the default

monet_db = read.csv(file = "monetdblogs.csv", header =  F);

mysql_db = read.csv(file = "std_bfr_mysql_log.csv", header =  F);
mm_mysql_db = read.csv(file = "mmysql_log.csv", header =  F);

cpp = read.csv(file = "prefetch_logs.csv", header =  F);


plot(monet_db$V1, monet_db$V4 ,type="o", col="blue", pch=2, lty=2, bty='L',  ylim=c(50,8000), ylab="Latency (seconds)" , xlab ="Scaling factor" )

points(mysql_db$V1, mysql_db$V4,  col="green", pch=15);
lines(mysql_db$V1, mysql_db$V4,  col="green",lty=3);

points(mm_mysql_db$V1, mm_mysql_db$V4,  col="black", pch=20);
lines(mm_mysql_db$V1, mm_mysql_db$V4,  col="black",lty=4);

points(cpp$V1, cpp$V4,  col="red", pch=1);
lines(cpp$V1, cpp$V4,  col="red",lty=1);

par(xpd=TRUE)
legend(55, 9600, legend=c("monetdb","mysql(InnoDB)","mysql(memory)","lazy loading(c++)"),
       col=c("blue","green","black","red"), pch =c(2,15,20,1) , lty=c(2,3,4,1) , cex=0.6, horiz = T)
