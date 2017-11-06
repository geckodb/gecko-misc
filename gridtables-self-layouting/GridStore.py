class Grid:
    def __init__(self):
        self.performance = 0.0  #performance value for the user layout
        self.layout = []    #store the user layout
        self.layouts = []   #store the list of layouts
        self.match = [] #store the matching layout

    #check if the user layout is valid
    def validLayout(self, layout):
        for i in range(len(layout)):
            for j in range(len(layout[0])):
                if(layout[i][j] > 0):
                    valid = self.visit(i, j, layout)
                    if valid == False:
                        return False
        return True

    #for a passed grid index, check if is a part of a valid rectangle
    def visit(self,i, j, layout):
        num = layout[i][j]
        #dx, dy stores the lower corner of the rectangle
        dx = i
        dy = j
        for x in range(i,len(layout)): #check the x axis extreme of grid for the number
            if(layout[x][j] != num):
                break
            else:
                dx = x
        for y in range(j,len(layout[0])): # check y axis extreme for the number
            if(layout[i][y] != num):
                break
            else:
                dy = y
        #sx, sy store the upper corner of the rectangle
        sx = i
        sy = j
        for x in range(i,-1, -1): #check the x axis extreme of grid for the number
            if(layout[x][j] != num):
                break
            else:
                sx = x
        for y in range(j,-1, -1): # check y axis extreme for the number
            if(layout[i][y] != num):
                break
            else:
                sy = y
        #go from the upper corner of the rectangle to the lower corner and check if all the numbers are the same
        for x in range(sx, 1+dx):
            for y in range(sy, 1+dy):
                if layout[x][y] != num:
                    return False
        return True

    #return current layout
    def getLayout(self):
        return self.layout

    #set the layout if it is valid
    def setLayout(self, layout, show = False):
        if(self.validLayout(layout)):
            self.layout = layout
        else:
            if show:
                print("The layout is invalid!")

    #calculate the performance using the distance between the Grid posisition values
    def calPerformance(self, l1, l2):
        diff = 0
        #take the shorter list of the two and calculate difference
        for i in range(min(len(l1), len(l2))):
            for j in range(min(len(l1[0]), len(l2[0]))):
                diff += abs(l1[i][j] - l2[i][j])
        diff += ((len(l1)*len(l1[0]) - len(l2)*len(l2[0])))*2
        return diff

    #get the best performance layout with ref. to the user input
    def getPerformance(self):
        #store the layout that matches closely to the user input
        match = []
        #store the max difference
        matchVal = 10000000000 #some value close to inf
        #Loop through all thw layouts and find the best one
        for i in range(len(self.layouts)):
            val = self.calPerformance(self.layout, self.layouts[i]) #  calculate the performance of the given layout with all the layouts in the set
            #current layout is closer than the previous best
            if val < matchVal:
                matchval = val
                match = self.layouts[i]
        self.performance = matchval
        self.match = match
        return self.performance*1.0

    #default layouts
    def laySet(self):
        l1 = [[ 5, 5, 5, 5 ],
         [ 1, 1, 1, 0 ],
         [ 1, 1, 1, 0 ],
         [ 2, 2, 2, 0 ],
         [ 3, 3, 4, 4 ],
         [ 5, 5, 5, 5 ]]

        l2 = [[1, 1, 1],
              [2, 2, 3],
              [2, 2, 3]]

        l3 = [[0, 4, 4, 4],
              [3, 4, 4, 4],
              [3, 2, 2, 0]
              ]

        l4 = [[1, 1, 1, 1],
              [0, 0, 0, 0],
              [1, 1, 1, 1]
              ]
        self.layouts.append(l1)
        self.layouts.append(l2)
        self.layouts.append(l3)
        self.layouts.append(l4)

    #check if the layout user inputted is valid, if so calculate the performance
    def simulator(self,layout):
        self.setLayout(layout, True)
        #create default layouts
        self.laySet()
        matchVal = self.getPerformance()
        print("Performance value = ", matchVal)
        print("Matching grid with input = ", self.match)


'''a = Grid()
layout = [[1, 1, 0],
          [1, 1, 1],
          [0, 0, 0]]

a.simulator(layout)

'''
