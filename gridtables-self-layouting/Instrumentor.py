from GridStore import Grid #import the Grid class
import random
#import numpy
import matplotlib.pyplot as plt

#change the row of the layout
def changeRow(a, value, row):
    #take the user input layout
    layout = a.getLayout()
    row -= 1
    #change the values of the layout
    for y in range(len(layout[row])):
        layout[row][y] = value
    return layout

def changeCol(a, value, col):
    layout = a.getLayout()
    col -= 1
    #change the column values of the layout
    for x in range(len(layout)):
        layout[x][col] = value
    return layout

#change a particular index in the layout
def changeGridval(a, value, row, col):
    layout = a.getLayout()
    layout[row-1][col-1] = value
    return layout

#choose a rectangle and change all the values in the rectangle to the layout
def changeRectangle(a, value, x, y):
    layout = a.getLayout()
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
    #print(5)
    return layout

#expand the layout by a row
def enlargeGrid(a, value):
    layout = a.getLayout()
    val = [value for i in range(len(layout[0]))]
    #add row to the end of the layout
    layout.append(val)
    return layout

#shrink the layout by a row
def shrinkGrid(a):
    layout = a.getLayout()
    #remove last row from the layout
    layout.pop()
    print(layout)
    return layout

#create a loop for the
def loop(a,loopVal):
    plotx = []
    ploty = []
    for i in range(loopVal):
        #k = random.randint(2,5)
        k = 5
        #get the user layout
        layout = a.getLayout()
        #choose random row to modify
        row = random.randint(1,len(layout))
        #choose random column to modify
        col = random.randint(1,len(layout[0]))
        #choose a random value for the layout row, column modification
        value = random.randint(1,10)
        #store all the functions in a list
        kFunctions = [
            lambda: changeRow(a,value, row),
            lambda: changeCol(a,value,col),
            lambda: changeGridval(a,value, row, col),
            lambda: enlargeGrid(a, value),
            lambda: changeRectangle(a, value, row, col)
            ]
        #Execute k functions
        for j in range(k):
            #call kth function
            layout = kFunctions[j]()
            #save the modified layout
            a.setLayout(layout)
            per = a.getPerformance()
        print("Loop ",i+1 ,"Performance value ", per)
        #store the performance for each iteration(to be displayed in the Y axis)
        ploty.append(per)
        #store the iteration number(to be displayed in the X axis)
        plotx.append(i+1)
        print("List = ", layout)
    #plot the graph based on the given values
    plotGraph(plotx, ploty)

#Plot the graph based on the XY axis
def plotGraph(px, py):
    plt.plot(px,py)
    plt.ylabel('Performance')
    plt.xlabel('Iteration')
    plt.show()


#create a Grid Layout
a = Grid()
#user input layout
layout =[[ 2, 2, 5, 0 ],
         [ 1, 1, 1, 0 ],
         [ 1, 1, 1, 0 ],]
a.simulator(layout)
#loop the layout and calculate performance
loop(a,7)
