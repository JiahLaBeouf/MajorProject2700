import numpy as np
from scipy.signal import savgol_filter
import matplotlib.pyplot as plt
import csv
import statistics

yOriginal = []
interval = []



with open('CSVDATA\code5.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = -1
    for row in csv_reader:
        if line_count == -1:
            line_count += 1
        else:
            yOriginal.append(row[0])
            interval.append(line_count)
            #print(f'{row[0]}')
            line_count += 1
    print(f'Processed {line_count} lines.')

ySmoothed = savgol_filter(yOriginal, 51, 3)

b = (np.diff(np.sign(np.diff(ySmoothed))) > 0).nonzero()[0] + 1
intervalNP = np.array(interval)
median = statistics.mean(b)

print("Median: ", median)
#print(ySmoothed[b])

minY = 10000
minX = 0

for i in range (0, len(ySmoothed)-1):
    if ySmoothed[i] < minY:
        minY = ySmoothed[i]
        minX = i
        
 

#for i in range(0, len(b)-1):
    #if ySmoothed[b[i]] > median:
    #    b = np.delete(b, i)
    #   i-=1

print(minX, minY)

#print(ySmoothed)
    
plt.plot(interval, yOriginal)
plt.plot(interval, ySmoothed, color='g')
plt.plot(minX, minY, color='r')

plt.plot(intervalNP[b], ySmoothed[b], "o", label="min", color='r')
plt.show()
