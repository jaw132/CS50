'''Description: To be used after import.py, given a Harry Potter house, returns the people in that house in alphabetical order
with their year of birth'''

# import libraries
from cs50 import SQL
from sys import argv, exit

# connect to database
db = SQL("sqlite:///students.db")

# check one parameter is provided, the house name
if len(argv) != 2:
    print("Invalid number of parameters, please enter one house")
    exit(1)

# Finding all the people in the house provided and returning their names and their year of birth.
for row in db.execute("select ""first"", ""middle"", ""last"", ""birth"" from students where house = ? order by last, first", argv[1]):
    if row['middle'] == '':
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
