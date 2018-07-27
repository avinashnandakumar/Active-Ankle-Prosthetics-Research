def readData(file):
  filename = file
  #raw_input("Please enter the name of the file you wish to read")
  file_obj = open(filename) # opens file
  x = file_obj.read()
  return x

x = readData("loop.txt")# call the function
#print x 
x2 = readData("error.txt")# call the function again
#print x2
x3 = readData("count.txt")# call the function again
#print x3
x4 = readData("command.txt")# call the function again
#print x4
x5 = readData("direction.txt")# call the function again
#print x4

def str2num(x):
  y=[]
  for i in x.split():
    y.append(float(i))
  return y

y=str2num(x)# changing the x2 values form string to numerical
y2=str2num(x2)
y3=str2num(x3)
y4=str2num(x4)
y5=str2num(x5)


# Taking numerical derivatives
def diff(y):
  x=[]
  for i in range(len(y)-1):
    x.append(y[i+1]-y[i])
  return x

def ref(counts, error):
  return [x+y for x, y in zip(counts, error)]

def sin(time):
	return (time/1)-(time*time*time)/6 + (time*time*time*time*time)/120 -(time*time*time*time*time*time*time)/5040+(time*time*time*time*time*time*time*time*time)/362880 -(time*time*time*time*time*time*time*time*time*time*time)/39916800+(time*time*time*time*time*time*time*time*time*time*time*time*time)/6227020800; 


      
derror = diff(y3)
derror.append(0)
import matplotlib.pyplot as plt
plt.subplot(511)
plt.plot(y, y2)
plt.plot(y, [0 for i in y])
plt.xlabel("LoopCount")
plt.ylabel("Error")

plt.subplot(512)
plt.plot(y, y3, '.-')
plt.plot(y, ref(y2, y3), '.')
plt.xlabel("LoopCount")
plt.ylabel("Counts")

plt.subplot(513)
plt.plot(y, y4)
plt.xlabel("LoopCount")
plt.ylabel("Command (Volts)")

plt.subplot(514)
plt.plot(y, y5)
plt.xlabel("LoopCount")
plt.ylabel("Derror")


plt.subplot(515)
plt.plot(y[0:6000], [sin(time*.001) for time in y[0:6000]])
plt.xlabel("LoopCount")
plt.ylabel("Derror Python")
plt.show()
