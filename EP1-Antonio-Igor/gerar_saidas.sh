#!/bin/bash
for i in `seq 1 6`; do
	./main entradas/entrada$i.txt > saidas/saida$i.txt
done
        
