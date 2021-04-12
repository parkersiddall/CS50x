from cs50 import SQL
from sys import argv
import csv


# check command line arguments
if len(argv) != 2:
    print("Incorrect input of files.")
    exit(1)

# declare db as our students database
open("students.db", "r").close()
db = SQL("sqlite:///students.db")

# declare argv[1] as house to make my code easier to read/write
house = argv[1]

    
# run a query to get a list of all the students in a given house. Returns it as a list where each row is a dictionary. SORTED last name, first name
roster = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

# print out each students fill name and birth year
for student in roster:
    if student["middle"] is None:
        print(student["first"], student["last"] + ",", "born", student["birth"])
    elif student["middle"] is not None:
        print(student["first"], student["middle"], student["last"] + ",", "born", student["birth"])







