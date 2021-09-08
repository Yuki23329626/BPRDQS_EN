import csv
import sys
import shlex
import os
filename = " ".join(map(shlex.quote, sys.argv[1:]))
print("filename: ", filename)

if(filename == "" or not os.path.exists(filename)):
    print("please type the correct file name of the log")
    exit()

data = open(filename, 'r')
lines = data.readlines()

outFileName = filename
base_time = 0

with open(outFileName, 'w') as f:
    for line in lines:
        string1 = line.split(" ")
        if(string1[1] == "at"):
            base_time = float(string1[2])
            break

with open(outFileName, 'w') as f:
    for line in lines:
        #if(count == 10):
        #    break
        string1 = line.split(" ")
        if(string1[1] == "at"):
            string1[2] = float(string1[2]) - base_time
            for item in string1:
                if item != string1[-1]:
                    f.write("%s " % item)
                else:
                    f.write("%s" % item)
        else:
            f.write(line)
