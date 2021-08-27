import numpy as np
import matplotlib.pyplot as plt
import sys, getopt

def main(argv):
    inputfile = ''
    try:
        #opts, args = getopt.getopt(argv, "hi:o:", ["ifile=", "ofile="])
        opts, args = getopt.getopt(argv, "h:i:", ["ifile="])
    except getopt.GetoptError:
        print('ssbSort.py -i <inputfile>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print ('ssbSort.py -i <inputfile>')
            sys.exit()
        elif opt in ("-i", "--ifile"):
            main.inputfile = arg
   #     elif opt in ("-o", "--ofile"):
   #         outputfile = arg
    print ('Input file is "', main.inputfile)
   # print ('Outputfile is "', outputfile)

if __name__ == "__main__":
    main(sys.argv[1:])

runNumber = np.loadtxt(main.inputfile, skiprows=1, dtype=int, usecols=[0])
sb0Counts = np.loadtxt(main.inputfile, skiprows=1, dtype=int, usecols=[1])
sb1Counts = np.loadtxt(main.inputfile, skiprows=1, dtype=int, usecols=[7])

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

#Sum SSB counts
sb0Total = 0
sb1Total = 0
if len(sb0Counts) == len(sb1Counts):
    arrLen = len(sb0Counts)

for i in range(0,arrLen):
    sb0Total = sb0Total + sb0Counts[i]
    sb1Total = sb1Total + sb1Counts[i]
textstr = '\n'.join((
    r'SB0 Total = %i' % (sb0Total, ),
    r'SB1 Total = %i' % (sb1Total, ),
))
# these are matplotlib.patch.Patch properties
props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)

# place a text box in upper left in axes coords
ax1.text(0.05,0.95, textstr, transform=ax.transAxes, fontsize=20, verticalalignment='top', bbox=props)
plt.show()
