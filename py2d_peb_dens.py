#!bin/python
from readcol import *
import matplotlib.pyplot as plt
import numpy as np

num=1

rad=readcol("rad_chart.txt")
size=readcol("size_chart.txt");
n_rad=len(rad)
n_size=len(size)

dens=np.ndarray(shape=(n_size,n_rad),dtype=float)
print np.shape(dens)
print np.shape(dens[0])
dens[0],dens[1],dens[2],dens[3],dens[4],dens[5],dens[6],dens[7],dens[8],dens[9],dens[10]=readcol("outp_sigma"+str(num)+".txt",twod=False)
rad,size=np.meshgrid(rad,size)

plt.contourf(rad,size,dens,cmap=plt.cm.jet)
plt.yscale("log")
plt.xlabel("r (AU)")
plt.ylabel("size (cm)")
plt.colorbar(label="\Sigma")
plt.show()
