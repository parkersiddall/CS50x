from sys import argv
from csv import reader, DictReader

#check to be sure command line arguments are correct(ish)
if len(argv) != 3:
    print("Incorrect input of files. Please insert database and dna sequence")
    exit(1)

# create an empty sequence where we can store the RTA types that we extract from the database
RTASequence = {}

# open csv file (database, argv[1]) and read its contents into memory, then extract RTA types
with open(argv[1]) as database:
    databasereader = reader(database)
    for i in databasereader:
        RTAtypes = i
        # take out the 'name' text to leave the RTA types
        RTAtypes.pop(0)
        #break out of the for loop so that only the first item is removed
        break

# for each RTAtype we have to plug it into the dictionary we made before.
for i in RTAtypes:
    RTASequence[i] = 0

# open txt file (argv[2]dna string) and read its contents into memory
with open(argv[2]) as dnafile:
    dnareader = reader(dnafile)
    # save it as a list. Iterates once since there is only one long item
    for i in dnareader:
        dna = i

#convert the list into a string
dnastring = dna[0]

# iterate through each STRtype and search for consecutive matches in the DNA string
for rta in RTASequence:
    # set length for each STR
    length = len(rta)
    # set some counters to keet track of current and highest tally
    tempcounter = 0
    maxcounter = 0

    # nested loop to go through each item of the DNA string
    for i in range(len(dnastring)): # with the range function c will be interpreted as a number instead of a letter

        # boolean to set temp counter back to 0 after a break in consecutive rta
        while tempcounter > 0:
            tempcounter = 0
            continue

        # check for matches using the length, if match is found it initiates a check to see how many consecutive matches there are by adjusting i
        while dnastring[i:i + length] == rta:
            tempcounter += 1
            i = i + length

            # update max counter if it is higher
            if tempcounter > maxcounter:
                maxcounter = tempcounter

    # add max to RTASequance value in dictionary to configure the total.
    RTASequence[rta] += maxcounter

# compare STR counts from dna sequence against the rows in the database to find a matching person
with open(argv[1]) as databasecheck:
    persons = DictReader(databasecheck)
    
    # loop to go through and check each persons matches
    for person in persons:
        #counter to keep track of how many rta types match the dna sequence
        counter = 0
        #loop to go through each rta type and check for matches
        for rta in RTASequence:
            if RTASequence[rta] == int(person[rta]): #convert to int because it is opened as a string in a dict
                counter +=1
                
            # if the number of matches is the counter is equal to the types of RTA then there a person is found
            if counter == len(RTASequence):
                print(person['name'])
                exit()
        
    # if the loop goes through and doesn't find any match it arrives here
    print("No match")
                
            
        
        