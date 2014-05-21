#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import Image
import string
import re
import array
import matplotlib.pyplot as plt

rep = 10

def plotagem(x,y,xerr,yerr):
	plt.figure()
	plt.errorbar(x, y, yerr=yerr, fmt='o')
	plt.title("m 10e-100")	
	plt.xlim([0,11])
	plt.ylim([-1,round(max(y)+max(yerr))+2])
	plt.plot([0, 11], [y[0], y[0]], color='red', linestyle='-', linewidth=1)

	plt.plot([0, 11.5], [0, 0], color='k', linestyle='-', linewidth=1)
	plt.plot([11.30, 11.5], [0.20, 0], color='k', linestyle='-', linewidth=1)
	plt.plot([11.30, 11.5], [-0.20, 0], color='k', linestyle='-', linewidth=1)

	plt.xlabel(u"nº de processadores")
	plt.ylabel(u"Duração média da execução (ms)")
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
		
