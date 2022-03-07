import numpy as np
import matplotlib.pyplot as plt

runs = np.loadtxt("pressure.dat", skiprows=1, usecols=[0])
p0= np.loadtxt("pressure.dat", skiprows=1, usecols=[1])
p1= np.loadtxt("pressure.dat", skiprows=1, usecols=[3])
p2= np.loadtxt("pressure.dat", skiprows=1, usecols=[5])


plt.plot(runs, p0, label="Mean Pressure")
plt.plot(runs, p1, label="first 2 mins")
plt.plot(runs, p2, label="last 2 mins")
plt.ylim(4.5,7)
plt.legend()
plt.show()
