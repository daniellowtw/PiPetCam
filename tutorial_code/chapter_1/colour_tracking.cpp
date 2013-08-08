#include "stdio.h"
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;
using namespace std;

int lowerH=0;
int lowerS=0;
int lowerV=0;

int upperH=180;
int upperS=256;
int upperV=256;

IplImage* imgThresh;
IplImage* imgHSV;
IplImage* frame;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){

	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV), cvScalar(upperH,upperS,upperV), imgThresh); 

	return imgThresh;
}

//This function create two windows and 6 trackbars for the "Settings" window
void setwindowSettings(){
	cvNamedWindow("Filtered", 2);
	cvNamedWindow("Original", 2);
	cvNamedWindow("Settings", 2);

	cvCreateTrackbar("LowerH", "Settings", &lowerH, 180, NULL);
	cvCreateTrackbar("UpperH", "Settings", &upperH, 180, NULL);

	cvCreateTrackbar("LowerS", "Settings", &lowerS, 256, NULL);
	cvCreateTrackbar("UpperS", "Settings", &upperS, 256, NULL);

	cvCreateTrackbar("LowerV", "Settings", &lowerV, 256, NULL);
	cvCreateTrackbar("UpperV", "Settings", &upperV, 256, NULL); 
}

int main(int argc, const char **argv){

	if (argc == 1) {
		fprintf(stderr, "Usage: colour_tracking [File]");
		return 0;
	}

	frame = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);
	if (!frame) return 0;
	
	setwindowSettings();
	//iterate through each frames of the video
	while(true){
	
		imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
		cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV

		imgThresh = GetThresholdedImage(imgHSV);

		cvShowImage("Filtered", imgThresh);
		cvShowImage("Original", frame);

		//Clean up used images
		cvReleaseImage(&imgHSV);
		cvReleaseImage(&imgThresh);

		//Wait 80mS
		int c = cvWaitKey(80);
		//If 'ESC' is pressed, break the loop
		if((char)c==27 ) break;

	}

	cvDestroyAllWindows();

	return 0;
}
