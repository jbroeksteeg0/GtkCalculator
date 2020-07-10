import random
import time
s = ""
c=0
for i in range(int(1e1)):
    
    if random.random() <= 0.2:
        s += "("
        c+=1
    s+= str(random.randint(1,10))
    
    if random.random() <= 0.2 and c>0:
        s += ")"
        c-=1
    s+=random.choice(["+","-","*"])



s+=str(random.randint(1,9))
for i in range(c):
    s+=")"

t = time.time()
print(eval(s))
print(s)
print(time.time()-t)
open("input","w").write(s)
