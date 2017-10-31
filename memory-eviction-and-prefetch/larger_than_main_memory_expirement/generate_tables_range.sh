#!/bin/bash
for ((sf=$1;sf <= $2;sf +=$3))
{
 echo "generating scaling factor : $sf";
sudo ./dbgen.sh $sf;
} 

