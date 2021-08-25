import numpy as np
import matplotlib.pyplot as plt


runNumber = np.loadtxt("output_ssb.dat", skiprows=1, dtype=int, usecols=[0])
sb0Counts = np.loadtxt("output_ssb.dat", skiprows=1, dtype=int, usecols=[1])
sb1Counts = np.loadtxt("output_ssb.dat", skiprows=1, dtype=int, usecols=[7])

fig = plt.figure()
ax=fig.add_subplot(111)
ax0=fig.add_subplot(211)
ax1=fig.add_subplot(212)

ax.spines['top'].set_color('none')
ax.spines['bottom'].set_color('none')
ax.spines['left'].set_color('none')
ax.spines['right'].set_color('none')
ax.tick_params(labelcolor='w', top=False, bottom=False, left=False, right=False)

ax0.plot(runNumber, sb0Counts, 'ro')
#ax0.set_xlabel('Run Number')
#ax0.set_ylabel('Counts')
ax0.set_title('SB0 Counts')
for x,y in zip(runNumber,sb0Counts):
    label = f"({x},{y})"
    ax0.annotate(label, # this is the text
                 (x,y), # these are the coordinates to position the label
                 textcoords="offset points", # how to position the text
                 xytext=(0,10), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center

ax1.plot(runNumber, sb1Counts, 'ro')
#ax1.set_xlabel('Run Number')
#ax1.set_ylabel('Counts')
ax1.set_title('SB1 Counts')
for x,y in zip(runNumber,sb1Counts):
    label = f"({x},{y})"
    ax1.annotate(label, # this is the text
                 (x,y), # these are the coordinates to position the label
                 textcoords="offset points", # how to position the text
                 xytext=(0,10), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center
# Set common labels
ax.set_xlabel('Run Number')
ax.set_ylabel('Counts')
plt.show()


