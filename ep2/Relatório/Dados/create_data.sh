#!/bin/sh
#1e-100
path="output/output_"
> $path$3.csv
for i in `seq 1 $1`
do
	str=""
	for j in `seq 1 $2`
	do
		str="$str`../../ep2 $i m 0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001 t`,"
	done
	str="$str;"
	echo $str >> $path$3.csv
done
