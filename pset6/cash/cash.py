from cs50 import get_float

#create true loop to continue prompting user until we like their input
while True:
    #ask for amount of change
    change_owed = get_float("Change owed: ")
    if change_owed > 0:
        break

#multiply change by 100 in order to make it an int instead of float to avoid problems
cents = round(change_owed * 100)

#declare new int for coin counter
coin_counter = 0

#loop through for quarters
while True:
    if cents >= 25:
        cents = cents - 25
        coin_counter += 1
    else:
        break

#loop through for dimes
while True:
    if cents >= 10:
        cents = cents - 10
        coin_counter += 1
    else:
        break

#loop through for nicks
while True:
    if cents >= 5:
        cents = cents - 5
        coin_counter += 1
    else:
        break

#same shit for pennies
while True:
    if cents >= 1:
        cents = cents - 1
        coin_counter += 1
    else:
        break
    
#print coin counter
print(coin_counter)


