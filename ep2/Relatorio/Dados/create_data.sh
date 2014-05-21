#!/bin/sh
#1e-100
path="output/output_"
> $path$2.csv
for i in `seq 1 $1`
do
	str=""
	for j in `seq 1 $2`
	do
		str="$str`../../ep2 $i m 1e-2000 t`,"
	done
	str="$str;"
	echo $str >> $path$2.csv
done
