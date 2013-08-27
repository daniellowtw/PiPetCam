#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat temp_frame, background_frame, frame_gray, background_frame_gray, subtracted_frame;
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)

int lowerH=0;
int lowerS=0;
int lowerV=0;

int upperH=180;
int upperS=256;
int upperV=256;

Mat colour_filtered_frame; //for filtering by colour
Mat frame_hsv; // HSV of frame
bool bColourFilter = false;

void setUpWindow(){
	namedWindow("Main", 1);
	namedWindow("Settings", CV_WINDOW_KEEPRATIO);
	createTrackbar("Threshold", "Settings", &slider_bg_subtract, 255, NULL);
	createTrackbar("LowerH", "Settings", &lowerH, 180, NULL);
	createTrackbar("UpperH", "Settings", &upperH, 180, NULL);
	createTrackbar("LowerS", "Settings", &lowerS, 256, NULL);
	createTrackbar("UpperS", "Settings", &upperS, 256, NULL);
	createTrackbar("LowerV", "Settings", &lowerV, 256, NULL);
	createTrackbar("UpperV", "Settings", &upperV, 256, NULL); 
}

Mat GetThresholdedImage(Mat imgHSV){
	Mat imgThresh;
	inRange(imgHSV, Scalar(lowerH,lowerS,lowerV, 0), Scalar(upperH,upperS,upperV, 0), imgThresh); 
	return imgThresh;
}

void doColourFilter(){
	colour_filtered_frame = GetThresholdedImage(frame_hsv);
}

void bg_subtract() {
	cvtColor(temp_frame, frame_gray, CV_BGR2GRAY);
	cvtColor(background_frame, background_frame_gray, CV_BGR2GRAY);
	GaussianBlur(frame_gray, frame_gray, Size(3,3), 3);
	GaussianBlur(background_frame_gray, background_frame_gray, Size(3,3), 3);
	absdiff(frame_gray, background_frame_gray, subtracted_frame);
	threshold( subtracted_frame, subtracted_frame, slider_bg_subtract, 255, THRESH_BINARY );
}

int main()
{
	VideoCapture cap(-1);
	if ( !cap.isOpened() )  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	setUpWindow();

	while (1)
	{
		cap >> temp_frame;
		cvtColor(temp_frame, frame_hsv, CV_RGB2HSV);

		if (bColourFilter) {
			imshow("Main", GetThresholdedImage(frame_hsv));
		}
		else {
			imshow("Main", temp_frame);
		}

		switch ((char) waitKey(30))
		{
		case ' ':
			temp_frame.copyTo(background_frame);
			namedWindow("Background", 1);
			imshow("Background", background_frame);
			break;
		case 'b':
			bg_subtract();
			namedWindow("Subtract", 1);
			imshow("Subtract", subtracted_frame);
		case 'a':
			bColourFilter = !bColourFilter;
			break;
		case (char) 27:
			return 1;
			break;
		default:
			break;
		}
	}
	return 1;
}