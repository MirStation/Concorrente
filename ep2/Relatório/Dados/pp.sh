#!/bin/sh

cont=0
factor=0
nofexp=5
anterior=0
echo "$1 $2"
for i in `seq 0 $1`
do
	factor=$(($i * $2))
	for j in `seq 1 $nofexp`
	do
		cont=$(($cont + `./parte2cl $factor`))
	done
	cont=$(($cont / $nofexp))
	echo "$cont $(($anterior / $cont))"
	anterior=`$cont * 1.0`
done
