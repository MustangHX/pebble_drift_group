#!bin/python
from readcol import *
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm
num=1

rad=readcol("rad_chart.txt")
size=readcol("size_chart.txt");
n_rad=len(rad)
n_size=len(size)

dens=np.ndarray(shape=(n_size,n_rad),dtype=float)
print dens
#print np.shape(dens)
#print np.shape(dens[0])
rad,size=np.meshgrid(rad,size)

X = 10*np.random.rand(5,3)
dens=np.random.rand(n_size,n_rad)
print dens
fig, ax = plt.subplots()
ax.imshow(X, cmap=cm.jet)#, interpolation='nearest')
print X
plt.show()
for i in range(0,10):
	num=i
#dens[0],dens[1],dens[2],dens[3],dens[4],dens[5],dens[6],dens[7],dens[8],dens[9],dens[10]=readcol("out_sigma"+str(num)+".txt",twod=False)
	dens=readcol("out_sigma"+str(num)+".txt",twod=False)
#	dens=np.loadtxt("out_sigma"+str(num)+".txt")
#	print dens

#	plt.scatter(rad,size,c=dens)
	plt.imshow(dens,cmap=plt.cm.jet,interpolation='nearest')
#	plt.yscale("log")
#	plt.xlabel("r (AU)")
#	plt.ylabel("size (cm)")
#	plt.colorbar(label="\Sigma")
	plt.show()
#	plt.contourf(dens,cmap=plt.cm.jet,interpolation='nearest')
	plt.scatter(rad,size,c=dens,facecolor=None)
	plt.yscale("log")
        plt.xlabel("r (AU)")
        plt.ylabel("size (cm)")
        plt.colorbar(label="\Sigma")
        plt.show()
