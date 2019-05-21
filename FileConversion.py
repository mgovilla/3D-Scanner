import math

def main():
    myFile = open("C:/Users/Mayank/Desktop/Point2.txt", "r")
    new = open("C:/Users/Mayank/Desktop/Point3.txt", "w+")

    lines = myFile.readlines()
    j = 1.0
    for i in lines:
        point = i.split(",")
        theta = float((j*1.8*math.pi) / 180.0) 

        if(j % 199 == 0):
            x = 0
            y = 0
        else:
            y = float(point[1]) / float(math.sin(theta))
            x = float(point[0]) / float(math.cos(theta))

        new.write('%f: %f %f \n' % (theta, x, y))
        if(j == 1):
            print(point[1])
        j = j + 1.0




if __name__ == "__main__":
    main()