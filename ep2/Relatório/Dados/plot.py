import sys
import Image
import string
import re
import array
import matplotlib.pyplot as plt

rep = 64
intervalo = 1000

def plotagem(a):
	fig = plt.figure()
	ax = fig.add_subplot(1,1,1)
 	x = [i*intervalo for i in range(rep)]
	y = a
	ax.bar(x,y)
	plt.show()


if __name__ == "__main__" :
	f = open(sys.argv[1],'r')
	a = [0 for i in range(rep)]

	for i in range(rep):
		a[i] = int(f.readline())

	plotagem(a)
		
