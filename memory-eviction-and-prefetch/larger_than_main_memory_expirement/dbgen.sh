#!/bin/bash
SF=${1:-1}
cd '/home/mohsen/tpch-dbgen'
echo "SF=$SF"

d=$SF
mkdir -p $d || exit 1
(
cd $d || exit 2
sudo  ../dbgen -v -f  -s $SF -b ../dists.dss "Y"
)
#echo "Convert to voltbd suitable files"

cd $SF

for i in `ls *.tbl`; 
 do chmod  777 $i; 
done;


#for i in `ls *.tbl`; 
# do sed 's/|$//' $i > ${i/tbl/csv}; 
# echo $i; 
#done;

