from cs50 import get_int

#promt user for a positive integer between 1-8 included
while True:
    n = get_int("Insert number:")
    if n > 0 and n <= 8:
        break

#create loop for rows
for i in range(n):
    #create loop for columns
    for j in range(n):
        if (i + j) < n - 1:
            print(" ", end = "")
        else:
            print("#", end = "")
    print()
            

