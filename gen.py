import random

s = ""
for i in range(10):
    s += str(int(random.random() * 1000)) + " * ";

s = s[:-2]
print(s)
print("%20.10f " % eval(s))