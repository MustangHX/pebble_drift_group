import matplotlib.pyplot as plt
from readcol import *

for i in range(0,40,1):
	in_name="pebble_num_"+str(i)+".txt"
	rad,yr,a1,a2=readcol(in_name,twod=False)

	plt.plot(rad,yr)
plt.yscale("log")
#plt.xscale("log")
plt.xlabel("r (AU)")
plt.ylabel("time (yr)")
plt.show()
