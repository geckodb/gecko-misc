from GridStorev2 import Grid #import the Grid class
import random
import copy
#import numpy
import matplotlib.pyplot as plt

#change the row of the layout
def changeRow(lay, value, row):
    layout = copy.deepcopy(lay)
    #take the user input layout
    row -= 1
    #change the values of the layout
    for y in range(len(layout[0])):
        layout[row][y] = value
    return layout

def changeCol(lay, value, col):
    layout = copy.deepcopy(lay)
    col -= 1
    #change the column values of the layout
    for x in range(len(layout)):
        layout[x][col] = value
    return layout

#change a particular index in the layout
def changeGridval(lay, value, row, col):
    layout = copy.deepcopy(lay)
    layout[row-1][col-1] = value
    return layout

#choose a rectangle and change all the values in the rectangle of the layout
def changeRectangle(lay, value, x, y):
    layout = copy.deepcopy(lay)
    x-=1
    y-=1
    #start x-axis index of the rectangle
    startx = x
    #start y-axis index of the rectangle
    starty = y
    #end x-axis index of the rectangle
    endx = x
    #end y-axis index of the rectangle
    endy = y
    num = layout[x][y]
    #go up the layout
    while startx > 0:
        if(layout[startx][y] == num):
            startx-=1
        else:
            startx += 1
            break
    #go left of the layout
    while starty > 0:
        if(layout[x][starty] == num):
            starty-=1
        else:
            starty += 1
            break
    #go downwards of the layout
    while endx < len(layout):
        if(layout[endx][y] == num):
            endx+=1
        else:
            endx -= 1
            break
    #go right of the layout
    while endy < len(layout[0]):
        if(layout[x][endy] == num):
            endy+=1
        else:
            break
    if endx >= len(layout):
        endx -=1
    if endy >= len(layout[0]):
        endy -=1
    #cover the rectangle and change all the values
    for i in range(startx, endx+1):
        for j in range(starty, endy+1):
            layout[i][j] = value
    return layout

#expand a rectangle in the layout
def enlargeGrid(lay, value):
    layout = copy.deepcopy(lay)
    #choose a random point in the layout
    x = random.randint(0, len(layout)-1)
    y = random.randint(0, len(layout[0])-1)
    #start x-axis index of the rectangle
    startx = x
    #start y-axis index of the rectangle
    starty = y
    #end x-axis index of the rectangle
    endx = x
    #end y-axis index of the rectangle
    endy = y
    num = layout[x][y]
    #go up the layout
    while startx > 0:
        if(layout[startx][y] == num):
            startx-=1
        else:
            startx += 1
            break
    #go left of the layout
    while starty > 0:
        if(layout[x][starty] == num):
            starty-=1
        else:
            starty += 1
            break
    #go downwards of the layout
    while endx < len(layout):
        if(layout[endx][y] == num):
            endx+=1
        else:
            endx -= 1
            break
    #go right of the layout
    while endy < len(layout[0]):
        if(layout[x][endy] == num):
            endy+=1
        else:
            break
    if endx >= len(layout):
        endx -=1
    if endy >= len(layout[0]):
        endy -=1
    #extend the rectangle in such a way that it fits inside the layout
    endx  = random.randint(endx, len(layout)-1)
    endy  = random.randint(endy, len(layout[0])-1)
    #cover the rectangle and change all the values
    for i in range(startx, endx+1):
        for j in range(starty, endy+1):
            layout[i][j] = value
    return layout

#shrink a rectangle in the layout
def shrinkGrid(lay):
    layout = copy.deepcopy(lay)
    x = random.randint(0, len(layout)-1)
    y = random.randint(0, len(layout[0])-1)
    #start x-axis index of the rectangle
    startx = x
    #start y-axis index of the rectangle
    starty = y
    #end x-axis index of the rectangle
    endx = x
    #end y-axis index of the rectangle
    endy = y
    num = layout[x][y]
    #go up the layout
    while startx > 0:
        if(layout[startx][y] == num):
            startx-=1
        else:
            startx += 1
            break
    #go left of the layout
    while starty > 0:
        if(layout[x][starty] == num):
            starty-=1
        else:
            starty += 1
            break
    #go downwards of the layout
    while endx < len(layout):
        if(layout[endx][y] == num):
            endx+=1
        else:
            endx -= 1
            break
    #go right of the layout
    while endy < len(layout[0]):
        if(layout[x][endy] == num):
            endy+=1
        else:
            break
    if endx >= len(layout):
        endx -=1
    if endy >= len(layout[0]):
        endy -=1
    #new end co-ordinates of the rectangle
    endxx = random.randint(startx,endx)
    endyy = random.randint(starty,endy)
    #cover the rectangle and change all the values
    for i in range(startx, endx+1):
        for j in range(starty, endy+1):
            if(i <= endxx or j <= endyy):
                continue
            else:
                layout[i][j] = 0;
    return layout

#Modify the default layouts
def loopDefaultLayout(a):
    #change for all layouts
    for lay in range(a.getLayoutCount()):
        k = 6
        #get the default layout
        layout = list(a.getDefLayout(lay))
        #print("Def layout", lay, " = ",  a.getDefLayout(lay))
        row = random.randint(1,len(layout))
        col = random.randint(1,len(layout[0]))
        value = random.randint(1,10)
        kFunctions = [
            lambda: changeRow(layout, random.randint(1,10), row),
            lambda: changeCol(layout, random.randint(1,10),col),
            lambda: shrinkGrid(layout),
            lambda: changeGridval(layout, random.randint(1,10), row, col),
            lambda: enlargeGrid(layout, random.randint(1,10)),
            lambda: changeRectangle(layout, random.randint(1,10), row, col),
            ]
        for j in range(k):
            layout = kFunctions[j]()
            if(a.validLayout(layout)):
                a.setDefLayout(layout, lay)
        #print("AFTER CHANGE : Def layout", lay, " = ",  a.getDefLayout(lay)) #Remove comment to see change in layouts
        #After the changes have been made to a layout, print if the result layout is a valid one or not
        if(a.validLayout(layout)):
            print("Default Layout", lay, "is valid after change. Changed layouut = ", layout)
        else:
            print("Default Layout", lay, " is invalid after change. Changed layout = ", layout)


#create a loop for the
def loop(a,loopVal):
    plotx = []
    ploty = []
    for i in range(loopVal):
        #k = random.randint(2,5)
        k = 6
        #get the user layout
        layout = copy.deepcopy(a.getLayout())
        #choose random row to modify
        row = random.randint(1,len(layout))
        #choose random column to modify
        col = random.randint(1,len(layout[0]))
        #choose a random value for the layout row, column modification
        value = random.randint(1,10)
        #store all the functions in a list
        kFunctions = [
            lambda: changeRow(layout, random.randint(1,10), row),
            lambda: changeCol(layout, random.randint(1,10),col),
            lambda: shrinkGrid(layout),
            lambda: changeGridval(layout, random.randint(1,10), row, col),
            lambda: enlargeGrid(layout, random.randint(1,10)),
            lambda: changeRectangle(layout, random.randint(1,10), row, col),
            ]
        #Execute k functions
        for j in range(k):
            #call kth function
            layout = kFunctions[j]()
            #save the modified layout
            if a.validLayout(layout):
                a.setLayout(layout)
            per = a.getPerformance()
        print("Loop ",i+1 ,"Performance value ", per) # Remove comment to see performances
        #store the performance for each iteration(to be displayed in the Y axis)
        ploty.append(per)
        #store the iteration number(to be displayed in the X axis)
        plotx.append(i+1)
        #print("List = ", a.getLayout())
        #Check if the performance value forms a pattern
        #we choose i > 16 because we need a minimum set of numbers to find a pattern(er assume the min set as 16).
        if i > 16 and pattern(ploty):
            break
    #plot the graph based on the given values
    plotGraph(plotx,ploty)

#find a pattern in the graph based on the plotting values.
#since the numbers are a bit random, we don't find pattern in numbers
#we look for the performance value's rise and drop patterns(up-down)
def pattern(ploty):
    accuracy = 72.0/100.0 #percentage accuracy for pattern matching. We choose a value between 65 - 74 for better accuracy since any value lower than that will not be a pattern and higher values might lead to the program running forever since the patterns are random
    match1 = 0.0 #the match percentage of the current graph values
    match2 = 0.0 #sometimes the graph may have matching values, but shifted by an index. match 2 checks for the shifted matches
    binary = []
    for i in range(len(ploty)):
        if i == 0:
            binary.append(0)
        elif ploty[i] > ploty[i-1]:
            binary.append(1)
        else:
            binary.append(0)
    # we check for patterns from second half of the binary array, to the first half
    j = int(len(ploty)/2)
    for i in range(int(len(ploty)/2)):
        if(binary[i] == binary[j]):
            match1 += 2.0 # two numbers match
        if(binary[i+1] == binary[j]):
            match2 += 2.0
        j+=1
        if j == len(ploty):
            break
    match2 = match2*1.0/(len(ploty))
    match1 = match1*1.0/(len(ploty))
    if match1 >= accuracy or match2 >= accuracy:
        return True
    return False

#Plot the graph based on the XY axis
def plotGraph(px, py):
    plt.plot(px,py)
    plt.title('GridStore Simulator Plots')
    plt.ylabel('Performance')
    plt.xlabel('Iteration')
    plt.show()

#create a Grid Layout
a = Grid()
#user input layout

layout =[[ 2, 2, 5 ],
         [ 1, 1, 1 ],
         [ 1, 1, 1 ]]

#layout =   [[8, 8, 8, 8], [8, 8, 8, 8], [8, 8, 8, 8], [8, 8, 8, 0], [3, 3, 1, 4], [5, 5, 1, 5]]
a.simulator(layout)
#loop the layout and calculate performance
print("Exe : ")
loopDefaultLayout(a)
#200 is the max number of iterations we need. sometimes if the accuracy value is too high, no match might occur so 200 is the upper bound
loop(a,200)
