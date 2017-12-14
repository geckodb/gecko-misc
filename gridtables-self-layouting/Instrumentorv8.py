from GridStorev8 import Grid #import the Grid class
import random
import copy
#import numpy
import matplotlib.pyplot as plt
#install scipy with "pip install scipy-stack"
from scipy.spatial import distance

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

#return the start and end points of the rectangle
def getRecPoints(layout):
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
    return [startx, starty, endx, endy]

#choose a rectangle and change all the values in the rectangle of the layout
def changeRectangle(lay, value):
    layout = copy.deepcopy(lay)
    #get the start and end points of a random rectangle in the layout
    points = getRecPoints(layout)
    #start x-axis index of the rectangle
    startx = points[0]
    #start y-axis index of the rectangle
    starty = points[1]
    #end x-axis index of the rectangle
    endx = points[2]
    #end y-axis index of the rectangle
    endy = points[3]
    #cover the rectangle and change all the values
    for i in range(startx, endx+1):
        for j in range(starty, endy+1):
            layout[i][j] = value
    return layout

#expand a rectangle in the layout
def enlargeGrid(lay, value):
    layout = copy.deepcopy(lay)
    #get the start and end points of a random rectangle in the layout
    points = getRecPoints(layout)
    #start x-axis index of the rectangle
    startx = points[0]
    #start y-axis index of the rectangle
    starty = points[1]
    #end x-axis index of the rectangle
    endx = points[2]
    #end y-axis index of the rectangle
    endy = points[3]
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
    #get the start and end points of a random rectangle in the layout
    points = getRecPoints(layout)
    #start x-axis index of the rectangle
    startx = points[0]
    #start y-axis index of the rectangle
    starty = points[1]
    #end x-axis index of the rectangle
    endx = points[2]
    #end y-axis index of the rectangle
    endy = points[3]
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
    file = open("log.txt", "w")
    for lay in range(a.getLayoutCount()):
        k = random.randint(1, 6)
        #get the user layout
        layout = list(a.getDefLayout(lay))
        row = random.randint(1,len(layout))
        col = random.randint(1,len(layout[0]))
        value = random.randint(1,10)
        kFunctions = [
            lambda: changeRow(layout, random.randint(1,10), row),
            lambda: changeCol(layout, random.randint(1,10),col),
            lambda: shrinkGrid(layout),
            lambda: changeGridval(layout, random.randint(1,10), row, col),
            lambda: enlargeGrid(layout, random.randint(1,10)),
            lambda: changeRectangle(layout, random.randint(1,10)),
            ]
        FunctionName = [
            "Row change",
            "Column change",
            "Shrink grid",
            "Change grid value",
            "Enlarge grid",
            "Modify rectangle"
            ]
        file.write("\nLayout " + str(lay+1) + "\n\n" )
        for j in range(k):
            layout = kFunctions[j]()
            defLayout = a.getDefLayout(lay)
            valid = False
            if(a.validLayout(layout)):
                a.setDefLayout(layout, lay)
                valid = True
            file.write("Before layout = " + str(defLayout) + "\n")
            file.write("After layout = " + str(layout) + "\n\n")
            file.write("Valid = " + str(valid) + "\n\n")
            file.write("Actions\Modifications applied = " + FunctionName[j] + "\n\n")
        #print("AFTER CHANGE : Def layout", lay, " = ",  a.getDefLayout(lay)) #Remove comment to see change in layouts
        #After the changes have been made to a layout, print if the result layout is a valid one or not
        if(a.validLayout(layout)):
            file.write("Default Layout "+ str(lay) + "is valid after change. Changed layout = " + str(layout) + "\n")
        else:
            file.write("Default Layout" + str(lay) + " is invalid after change. Changed layout = " + str(layout) + "\n")
    file.close()

#create a loop for the user layout execution
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
            lambda: changeRectangle(layout, random.randint(1,10)),
            ]
        #Execute k functions
        for j in range(k):
            #call kth function
            layout = kFunctions[j]()
            #save the modified layout
            if a.validLayout(layout):
                a.setLayout(layout)
            per = a.getPerformance()
        #print("Loop ",i+1 ,"Performance value ", per) # Remove comment to see performances
        #store the performance for each iteration(to be displayed in the Y axis)
        ploty.append(per)
        #store the iteration number(to be displayed in the X axis)
        plotx.append(i+1)
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

'''return the distance btw the two layouts.
To calculate the distance, the matrices should have the same number of rows and columns.
Here we already have the same number of rows and columns in user and target layout.
In case if we do not have same number,we create Layouts a,b that have max Row and Max col size of both the layouts and copy the layouts into the matrix.
We use Frobenius Norm(http://mathworld.wolfram.com/FrobeniusNorm.html) to calculate the distance btw 2 matrices
'''
def calDistance(layout1, layout2):
    maxRow = max(len(layout1), len(layout2))
    maxCol = max(len(layout1[0]), len(layout2[0]))
    a = [[0 for x in range(maxCol)] for y in range(maxRow)]
    b = [[0 for x in range(maxCol)] for y in range(maxRow)]
    for x in range(len(layout1)):
        for y in range(len(layout1[0])):
            a[x][y] = layout1[x][y]

    for x in range(len(layout2)):
        for y in range(len(layout2[0])):
            b[x][y] = layout2[x][y]
    #distance btw the 2 layouts
    dis = 0
    #Calculate the difference btw 2 matrix
    for i in range(len(a)):
        for j in range(len(a[0])):
            a[i][j] = a[i][j] - b[i][j]
    #aInv stores the  conjugate transpose of matrix A
    aInv = [[0 for x in range(len(a))] for y in range(len(a[0]))]
    for i in range(len(a)):
        for j in range(len(a[0])):
            aInv[j][i] = a[i][j]
    #result for matrix multiplication btw A and A-Inverse
    matrixMul = [[0 for x in range(len(a))] for y in range(len(aInv[0]))]
    for i in range(len(a)):
        for j in range(len(aInv[0])):
            for k in range(len(aInv)):
                matrixMul[i][j] += a[i][k] * aInv[k][j]
    #trace is the  sum of the elements on the main diagonal of matrix A
    trace = 0
    for i in range(len(a)):
        if i >= len(a[0]):
            break;
        trace += matrixMul[i][i]
    dis = trace**(.5)
    #print("dis - ", dis) #uncomment this to print the distance of matrix in each loop
    return dis

#Plot the graph based on the XY axis
def plotGraph(px, py):
    plt.plot(px,py)
    plt.ylabel('Performance')
    plt.xlabel('Iteration')
    plt.show()


''' Here we will call Target layout and apply modifications on user layout and calculate distance.
    By looping this function for 500 iterations, minimum distance will be calculated bewteen 2 matrices.
'''
def matchTarget(a, loopVal):
    target = a.getTarget()
    distance = calDistance(a.getLayout(), a.getTarget())
    #store the layout that has the min distance to the target layout
    matchLayout = []
    file = open("targetLog.txt", "w")
    #print("Initial Distance btw Target and user layout = ", distance)
    file.write("Initial Distance btw Target and user layout = " + str(distance) + "\n")
    for i in range(loopVal):
        if distance == 0:
            break
        k = 6
        #get the user layout
        #match layout contains the minimum distance between the user layout and the target.
        if len(matchLayout) != 0:
            layout = copy.deepcopy(matchLayout)
        else:
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
            lambda: changeRectangle(layout, random.randint(1,10)),
            ]

        FunctionName = [
            "Row change",
            "Column change",
            "Shrink grid",
            "Change grid value",
            "Enlarge grid",
            "Modify rectangle"
            ]

        #Execute k functions
        for j in range(k):
            #call kth function
            if len(matchLayout) != 0:
                layout = copy.deepcopy(matchLayout)
            layout = kFunctions[j]()
            #if the layout is valid, check it with the target layout and see if match
            if a.validLayout(layout):
                a.setLayout(layout)
                dist = calDistance(layout, a.getTarget())
                file.write("Actions\Modifications applied = " + FunctionName[j] + "\n")
                file.write("Layout = "+ str (layout) + ", Distance = " + str(dist) + "\n\n")
                if(dist < distance):
                    distance = dist
                    matchLayout = copy.deepcopy(layout)
                    file.write("User layout(Modified) = " + str(matchLayout) + "\n\n")
                if dist == 0:
                    break
                #print(matchLayout, a.getTarget(), " -- ", distance)
    #print("User layout with min distance to Target= ", matchLayout, "\nDistance =", distance)
    file.write("User layout with min distance to Target= " + str(matchLayout) + "\nDistance =" + str(distance) + "\n")

#create a Grid Layout
a = Grid()

#user input layout
#layout =[[ 2, 2, 5, 5 ], [ 1, 1, 1, 4 ], [ 1, 1, 1, 4 ]]
#layout =   [[8, 8, 8, 8, 8], [8, 8, 8, 8, 8], [3, 3, 3, 3, 3], [8, 8, 8, 8, 8], [3, 3, 1, 4, 1], [5, 5, 1, 5, 1]]
layout =   [[8, 8, 8, 8, 8], [8, 8, 8, 8, 8], [3, 3, 3, 3, 3], [4, 4, 1, 4, 1], [3, 3, 1, 4, 1]]

a.simulator(layout)
#loop the layout and calculate performance
#loopDefaultLayout(a)
#200 is the max number of iterations we need. sometimes if the accuracy value is too high, no match might occur so 200 is the upper bound
#loop(a,200)

#loop the matchTarget and calculate distance between user and target layout
matchTarget(a, 500)
