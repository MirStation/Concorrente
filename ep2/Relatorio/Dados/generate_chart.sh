#!/bin/sh

./create_data.sh 10 1000
./create_plot_values.pl
./plot.py output/data.csv
