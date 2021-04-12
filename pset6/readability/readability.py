from cs50 import get_string

# prompt user to insert string
text = get_string("Text:")

# initialize counters for letters, words, and sentences
letters = 0
words = 1 
sentences = 0

# pull out char from string
for c in text:
    # check to see if it is a letter
    if c.isalpha():
        letters += 1
    # check to see if it is the end of a word
    if c == " ":
        words += 1
    # check to see if it is the end of a sentence.
    if c == "." or c == "!" or c == "?":
        sentences += 1

# creating the values for the equation 
l = (float(letters) / float(words)) * 100
s = (float(sentences) / float(words)) * 100

# running it through the equation
index = 0.0588*l - 0.296*s - 15.8

# round it out
index = round(index)

# print the index
if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")