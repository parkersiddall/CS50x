SELECT first, middle, last FROM students WHERE house = "argv[2]" ORDER BY last, first;




roster = db.execute("SELECT first, middle, last FROM students WHERE house = ? ORDER BY last, first", )

print(roster)