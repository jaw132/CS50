'''Description: Uploading a csv to a database table using sqlite3 and python'''

# importing libraries
from cs50 import SQL
from sys import argv, exit
import csv

# Connecting to database
db = SQL("sqlite:///students.db")

# Checking only one parameter is provided, the csv.
if len(argv) != 2:
    print("Invalid number of parameters entered, please provide one parameter")
    exit(1)

# Reading the csv, splitting the name into first, middle and last name and then inserting into table
with open(argv[1], 'r') as csvfile:
    stu_reader = csv.DictReader(csvfile, delimiter=',')

    for row in stu_reader:
        name = row["name"]
        house = row["house"]
        birth = row["birth"]

        namelist = []
        startindex = 0
        for i in range(len(name)):
            if name[i] == ' ':
                namelist.append(name[startindex:i])
                startindex = i+1
        namelist.append(name[startindex:])

        if len(namelist) == 2:
            namelist.append(namelist[1])
            namelist[1] = ''

        db.execute("insert into students (first, middle, last, house, birth) values (?, ?, ?, ?, ?)",
        namelist[0], namelist[1], namelist[2], house, birth)
