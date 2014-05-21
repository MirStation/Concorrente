#!/bin/sh

./create_data.sh 10 500 5
./create_plot_values.pl
./plot.py output/data.csv
