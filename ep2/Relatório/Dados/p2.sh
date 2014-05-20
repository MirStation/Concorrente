#!/bin/sh

for i in `seq 1 $1`
do
	../../ep2 $i m 0.0000001 > sample.s
done
