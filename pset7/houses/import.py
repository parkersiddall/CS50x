from cs50 import SQL
from sys import argv
import csv



# check to be sure that the command line arguments are correct
if len(argv) != 2:
    print("Incorrect input of files.")
    exit(1)

# declare db as our students database
db = SQL("sqlite:///students.db")

# open csv file, read it one line at a time. 
with open(argv[1], "r") as file:
    reader = csv.DictReader(file)
    
    #loop through each student in file
    for row in reader:
        
        #split their name by the spaces
        row["split_name"] = row["name"].split(" ")
        
        #conditional IF statement based on how many items are now in the split_names column
        if len(row["split_name"]) is 3:
            row["first_name"] = row["split_name"][0]
            row["middle_name"] = row["split_name"][1]
            row["last_name"] = row["split_name"][2]
            
        elif len(row["split_name"]) is 2:
            row["first_name"] = row["split_name"][0]
            row["middle_name"] = None
            row["last_name"] = row["split_name"][1]

        

        # insert student into databa
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", row["first_name"], row["middle_name"], row ["last_name"], row["house"], int(row["birth"]))