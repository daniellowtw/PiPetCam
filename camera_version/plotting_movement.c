#include "plotting_movement.h"

int abs(int n) { return n > 0 ? n : -n; }

void onMouse( int event, int x, int y, int qqq, void* qqqq) {
	if( selectObject ) {
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = abs(x - origin.x);
		selection.height = abs(y - origin.y);
	}
	switch( event ) {
	case CV_EVENT_LBUTTONDOWN:
		origin = cvPoint(x,y);
		selection = cvRect(x,y,0,0);
		selectObject = 1;
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = 0;
		if( selection.width > 0 && selection.height > 0 )
			trackObject = -1;
		break;
	}
}

void drawRectangles(IplImage* frame){
	CvPoint p1,p2;
	p1.x = selection.x;
	p2.x = selection.x + selection.width;
	p1.y = selection.y;
	p2.y = selection.y + selection.height;
	cvRectangle(frame,p1,p2,CV_RGB(255,0,0),1,CV_AA,0);
}

IplImage* GetThresholdedImage(IplImage* imgHSV){

	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV, 0), cvScalar(upperH,upperS,upperV, 0), imgThresh); 

	return imgThresh;
}

void background_subtract_switch_callback(int pos){}

void capture_background_switch_callback(int pos){
	if (capture_background_switch_value){
		background_frame_half = cvCreateImage(halfCvSize(frame_size),8, 3);
		background_frame = cvCreateImage(frame_size, 8, 1); // use gray scale because subtraction with colour is just horrendous.
		cvResize( temp_frame, background_frame_half , 0.5 );
		cvCvtColor(temp_frame, background_frame, CV_BGR2GRAY);

		//cvPyrDown(frame, background_frame);
		//cvNamedWindow("Background");
		//cvShowImage("Background", background_frame_half);
		background_plot = cvCloneImage(background_frame); // Set background plot to draw on image
	}
	else cvDestroyWindow("Background");
}

void setUpWindow(){
	cvNamedWindow("Main", CV_WINDOW_KEEPRATIO);
	cvNamedWindow("Settings", CV_WINDOW_KEEPRATIO);
	cvNamedWindow("Background_plot", CV_WINDOW_KEEPRATIO);
	cvCreateTrackbar("Cap BG", "Settings", &capture_background_switch_value, 1, capture_background_switch_callback);
	cvCreateTrackbar("BG Subtract", "Settings", &background_subtract_switch_value, 1, background_subtract_switch_callback);
	cvCreateTrackbar("Threshold", "Settings", &slider_bg_subtract, 255, NULL);
	cvCreateTrackbar("Min Area", "Settings", &slider_min_area, 100000, NULL);
	cvCreateTrackbar("LowerH", "Settings", &lowerH, 180, NULL);
	cvCreateTrackbar("UpperH", "Settings", &upperH, 180, NULL);
	cvCreateTrackbar("LowerS", "Settings", &lowerS, 256, NULL);
	cvCreateTrackbar("UpperS", "Settings", &upperS, 256, NULL);
	cvCreateTrackbar("LowerV", "Settings", &lowerV, 256, NULL);
	cvCreateTrackbar("UpperV", "Settings", &upperV, 256, NULL); 
	cvSetMouseCallback( "Main", onMouse, 0 );
}

void setUpImage(CvSize size){
	fWidth = size.width;
	fHeight = size.height;
	background_plot = cvCreateImage(size,8, 1);
	temp_frame_hsv = cvCreateImage(size,8, 3);
	frame = cvCreateImage(size,8, 3);
	cvZero(background_plot);

}

int plotting_movement(IplImage* cam_frame){
	
	frame_size = cvGetSize(cam_frame);
	setUpImage(frame_size);
	fWidth = frame_size.width;
	fHeight = frame_size.height;

	temp_frame = cam_frame; // get next frame
	cvCvtColor(temp_frame, temp_frame_hsv, CV_BGR2HSV);
	drawRectangles(temp_frame);
	if (bColourFilter) {
		cvShowImage("Main", GetThresholdedImage(temp_frame_hsv));
	}
	else {
		cvShowImage("Main", temp_frame);
	}
	cvShowImage("Background_plot", background_plot);

	c = (char) cvWaitKey(80);
	if (c == 'a') bColourFilter = !bColourFilter;
	if (c == 'b') background_subtract_switch_callback(1);

	return 1;
}

void exit_program(){
	cvDestroyAllWindows();
}

void print_stuff(){
	fprintf(stderr, "hello world\n");
}

CvSize halfCvSize( CvSize size){
	return cvSize(size.width, size.height);
}