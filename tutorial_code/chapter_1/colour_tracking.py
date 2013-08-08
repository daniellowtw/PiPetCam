# initialise Original capture
import cv2
import numpy as np
import sys


def do_nothing(*arg):
    # print arg
    pass

def setwindowSettings():
    # initial values
    lowerH = 0
    lowerS = 0
    lowerV = 0
    upperH = 180
    upperS = 256
    upperV = 256
    cv2.namedWindow("Settings", 2)
    cv2.namedWindow("Original", 2)
    cv2.namedWindow("Filtered", 2)
    cv2.createTrackbar('LowerH', "Settings", lowerH, 180, do_nothing)
    cv2.createTrackbar('UpperH', "Settings", upperH, 180, do_nothing)
    cv2.createTrackbar('LowerS', "Settings", lowerS, 256, do_nothing)
    cv2.createTrackbar('UpperS', "Settings", upperS, 256, do_nothing)
    cv2.createTrackbar('LowerV', "Settings", lowerV, 256, do_nothing)
    cv2.createTrackbar('UpperV', "Settings", upperV, 256, do_nothing)


def getThreshImage(image):
        
    imgHSV = cv2.cvtColor(image, cv2.cv.CV_RGB2HSV)
    lowerH = cv2.getTrackbarPos('LowerH', 'Settings')
    lowerS = cv2.getTrackbarPos('LowerS', 'Settings')
    lowerV = cv2.getTrackbarPos('LowerV', 'Settings')
    
    upperH = cv2.getTrackbarPos('UpperH', 'Settings')
    upperS = cv2.getTrackbarPos('UpperS', 'Settings')
    upperV = cv2.getTrackbarPos('UpperV', 'Settings')
    result = cv2.inRange(imgHSV, np.array((lowerH, lowerS, lowerV)), np.array((upperH, upperS, upperV)))
    
    # print time.time() - t1
    return result
    # return cv2.inRange(imgHSV, np.array((lowerH, lowerS, lowerV)), np.array((upperH, upperS, upperV)))


def main():

    # return if no file path given
    if (len(sys.argv) == 1):
        return

    setwindowSettings()

    while(1):
        frame = cv2.imread(sys.argv[1])
        cv2.imshow("Filtered", getThreshImage(frame))
        cv2.imshow("Original", frame)

        c = cv2.waitKey(80)
        if c == 27:
            break
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()