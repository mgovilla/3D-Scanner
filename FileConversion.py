import math

def main():
    # Open the Data from the file
    myFile = open("C:/Users/Mayank/Desktop/Point2.txt", "r")

    # Create new file for converted data
    new = open("C:/Users/Mayank/Desktop/Point3.txt", "w+")

    # Store all of the data in lines
    lines = myFile.readlines()

    j = 1.0
    for i in lines:
        point = i.split(",") # Data is in X,Y,Z Format
        theta = float((j*1.8*math.pi) / 180.0) 

        if(j % 199 == 0): # 200 values of theta per revolution 
            x = 0
            y = 0
        else:
            y = float(point[1]) / float(math.sin(theta))
            x = float(point[0]) / float(math.cos(theta))

        # Write new values to file
        new.write('%f: %f %f \n' % (theta, x, y)) 
        if(j == 1):
            print(point[1])
        j = j + 1.0




if __name__ == "__main__":
    main()