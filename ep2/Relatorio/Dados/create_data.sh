#!/bin/sh
#1e-100
path="output/output.csv"
> $path
for i in `seq 1 $1`
do
	str=""
	for j in `seq 1 $2`
	do
		str="$str`../../ep2 $i f 1e-500 t`,"
	done
	str="$str;"
	echo $str >> $path
	echo "$i done"
done
