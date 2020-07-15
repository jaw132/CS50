'''Description: Find who a given sequence of DNA belongs to, based on Short Tandem Repeats (STRs) analysis. STR are short sequences
of DNA was repeat themselves consectively. Individuals show high diversity is the number of repeats of STRs so they are a good way of 
identifying people. The input parameters are a csv containing a number of people and how often particular STRs repeat themselves and 
a text file of a sequence of DNA.'''

# import libaries
from sys import argv, exit
import csv

# Check 2 parameters are given
if len(argv) != 3:
    print("Invalid number of parameters entered")
    exit(1)

# Reading the csv file, appending the STRs to header and appending the names and the repeat values of each STR to rows
header, rows = [], []
with open(argv[1]) as people:
    peoreader = csv.reader(people, delimiter=',')
    row_count = 0
    for row in peoreader:
        if row_count == 0:
            header.append(row)
        else:
            rows.append(row)
        row_count += 1

# Create nested dictionary with the STR as the primary key and names as the secondary keys. The values are how 
# times a STR repeats for each person.
NoofSTR = len(header[0])
dataDict = {}

for i in range(1, NoofSTR):
    dataDict[header[0][i]] = {}
    for j in range(len(rows)):
        dataDict[header[0][i]][rows[j][0]] = rows[j][i]

#read txt file with DNA sequence
with open(argv[2]) as dnaopen:
    dnastr = dnaopen.read()

seqlen = len(dnastr)
culprit = {}

# Initialise a dictionary where are person has zero value
for i in rows:
    culprit[i[0]] = 0

# Loop through the DNA txt file are find the longest repeating substring of each STR, if the length
# equals somebody's value then add one to their entry in the culprit dictionary
for i in range(1, NoofSTR):
    maxstr, currstr = 0, 0
    STR = header[0][i]
    STRlen = len(STR)

    #check for biggest substring
    index = 0
    while index < seqlen-STRlen:
        if dnastr[index:index+STRlen] == STR:
            currstr += 1
            index += STRlen
            if currstr > maxstr:
                maxstr = currstr
        else:
            index += 1
            currstr = 0

    for j in dataDict[STR]:
        if dataDict[STR][j] == str(maxstr):
            culprit[j] += 1

# If somebody matches on all STR then they are culprit
for i in culprit:
    if culprit[i] == NoofSTR-1:
        print(i)
        exit(1)

print("No match")
