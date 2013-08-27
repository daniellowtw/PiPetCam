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
Mat background_plot;
bool bColourFilter = false;

bool firstTime = true;
int slider_min_area = 3000; // Min area threshold. Ideally I want to loop through all the contours to filter small contours.
Point position; // To store current point value;
Point position_old; // To store current point value;

Point origin;
Rect selection; // Track user rectangular selection
Rect temp_selection; // Track user rectangular selection
bool selectObject = false; // Flag for whether user is selecting rect

static void onMouse( int event, int x, int y, int foo, void* bar) {
	if( selectObject ) {
		temp_selection.x = MIN(x, origin.x);
		temp_selection.y = MIN(y, origin.y);
		temp_selection.width = abs(x - origin.x);
		temp_selection.height = abs(y - origin.y);
	}
	switch( event ) {
	case CV_EVENT_LBUTTONDOWN:
		origin = Point(x,y);
		temp_selection = Rect(x,y,0,0);
		selectObject = true;
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = false;
		selection = temp_selection; // Saves our selection
		temp_selection = Rect(0,0,0,0); // hides our temp selection
		break;
	}
}

void drawRectangles(Mat frame, Rect selection, int colour_choice = 0){ 
	Point p1,p2;
	Scalar temp_colour;
	p1.x = selection.x;
	p2.x = selection.x + selection.width;
	p1.y = selection.y;
	p2.y = selection.y + selection.height;

	switch (colour_choice)
	{
	case 1:
		temp_colour = CV_RGB(0,255,0);
		break;
	case 2:
		temp_colour = CV_RGB(0,0,255);
		break;
	default:
		temp_colour = CV_RGB(255,0,0);
		break;
	}
	rectangle(frame,p1,p2, temp_colour,1,CV_AA,0);
}


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
	createTrackbar("Min Area", "Settings", &slider_min_area, 100000, NULL);
	setMouseCallback( "Main", onMouse, 0 );
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

void doProcessing(){

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	bitwise_and(subtracted_frame, colour_filtered_frame, subtracted_frame);
	findContours( subtracted_frame, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	Mat drawing = Mat::zeros( subtracted_frame.size(), CV_8UC1 );
	if( contours.size() > 0 )
		drawContours(drawing, contours, -1, Scalar(255, 255, 255), CV_FILLED);

	Moments m = moments(drawing, true);
	Point center(m.m10/m.m00, m.m01/m.m00);
	double area = m.m00;

	if ( area >= slider_min_area ) { // this is to mitigate false positive due to noise
		if (!firstTime){
			position_old = position;
		}
		else {
			firstTime = false;
			position_old.x = center.x;
			position_old.y = center.y;
		}

		position.x = center.x;
		position.y = center.y;
		line(background_plot, position_old, position, Scalar(255,255,255,0), 1, CV_AA, 0);

		// printf( "position: %d, %d\n", position.x, position.y );
		circle( background_plot, position, 5, cvScalar( 255,255, 0, 0 ), 2, 8, 0 );
		circle( background_plot, position, 1, cvScalar( 255,255, 0, 0 ), 2, 8, 0 );
	} else {
		position = Point( -1, -1 );
		firstTime = true;
	}
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
		drawRectangles(temp_frame, temp_selection, 0); //required for drawing user's selection
		drawRectangles(temp_frame, selection , 1);

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
			background_frame.copyTo(background_plot);
			break;
		case 'b':
			bg_subtract();
			doColourFilter();
			doProcessing();
			//namedWindow("Plot", 1);
			imshow("Plot", background_plot);
			namedWindow("Subtract", 1);
			imshow("Subtract", subtracted_frame);
			break;
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