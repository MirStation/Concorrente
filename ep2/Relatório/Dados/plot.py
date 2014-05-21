#!/usr/bin/python
import sys
import Image
import string
import re
import array
import matplotlib.pyplot as plt

rep = 10

def plotagem(x,y,xerr,yerr):
	fig = plt.figure()

	ax = fig.add_subplot(1,1,1)
	ax.errorbar(x, y, yerr=yerr, fmt='o')
	ax.set_title("m 10e-100")

	plt.show()


if __name__ == "__main__" :
	f = open(sys.argv[1],'r')

	x = [i for i in range(rep)]
	y = [0 for i in range(rep)]
	xerr = [0 for i in range(rep)]
	yerr = [0 for i in range(rep)]

	i = 0
	for line in f:
		numbers = line.split()
		x[i]    = float(numbers[0])
		y[i]    = float(numbers[1])
		yerr[i] = float(numbers[2])		
		i+=1

	plotagem(x,y,xerr,yerr)
		
