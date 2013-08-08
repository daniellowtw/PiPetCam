#include "plotting_movement.h"

static void onMouse( int event, int x, int y, int, void* ) {
	if( selectObject ) {
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);
		selection &= Rect(0, 0, fWidth, fHeight);
	}
	switch( event ) {
	case EVENT_LBUTTONDOWN:
		origin = Point(x,y);
		selection = Rect(x,y,0,0);
		selectObject = true;
		break;
	case EVENT_LBUTTONUP:
		selectObject = false;
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

void showMaskPart(const IplImage* src, const IplImage* mask, IplImage* result)
{
	/* src is the source image which you want to mask
	* mask is a single channel binary image as a mask
	* result is the image with the same size, depth, channel with src
	*/

	cvZero(result);
	CvSize sz = cvSize(src->width, src->height);	
	IplImage* refImg = cvCreateImage(sz, src->depth, src->nChannels);
	cvZero(refImg);
	cvOr(src, refImg, result, mask);
	cvReleaseImage(&refImg);

}

IplImage* GetThresholdedImage(IplImage* imgHSV){

	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV), cvScalar(upperH,upperS,upperV), imgThresh); 

	return imgThresh;
}

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

void background_subtract(){
	cvReleaseImageData(frame_gray);
	cvReleaseImageData(frame_hsv);

}

void background_subtract_switch_callback(int pos){
	if (background_subtract_switch_value){

		// for processing
		frame_gray = cvCreateImage(cvGetSize(temp_frame),8, 1);
		frame_hsv = cvCreateImage(cvGetSize(temp_frame),8, 3);

		cvCvtColor(temp_frame, frame_gray, CV_BGR2GRAY);
		cvCvtColor(temp_frame, frame_hsv, CV_BGR2HSV);

		cvSetImageROI (frame_hsv, selection);
		cvSetImageROI (frame_gray, selection);
		cvSetImageROI (background_frame, selection);
		cvSetImageROI (background_plot, selection);

		//Filter by colour
		colour_filtered_frame = cvCreateImage(cvGetSize(frame_hsv),8, 1);
		colour_filtered_frame = GetThresholdedImage(frame_hsv);
		//end filter by colour

		subtracted_frame = cvCreateImage(cvGetSize(frame_gray),8, 1);
		temp_contours = cvCreateImage(cvGetSize(frame_gray),8, 1);

		temp_frame_gray = cvCloneImage(frame_gray);
		temp_background_frame = cvCloneImage(background_frame); 
		cvSmooth(temp_frame_gray, temp_frame_gray, CV_GAUSSIAN, 3);
		cvSmooth(temp_background_frame, temp_background_frame, CV_GAUSSIAN, 3);
		cvAbsDiff(temp_frame_gray, temp_background_frame, subtracted_frame);

		//AND both binary image
		cvAnd(subtracted_frame, colour_filtered_frame, subtracted_frame);
		//end AND both binary image

		CvMemStorage*  g_storage  = NULL;
		CvSeq* contours = 0;
		CvMoments *moments = (CvMoments*) malloc(sizeof(CvMoments));

		//TODO 
		//if( g_storage==NULL ) {
		g_storage = cvCreateMemStorage(0);
		//} else {
		//cvClearMemStorage( g_storage );
		//}

		cvNamedWindow("Subtracted");
		cvShowImage("Subtracted", subtracted_frame);

		cvThreshold( subtracted_frame, subtracted_frame, slider_bg_subtract, 255, CV_THRESH_BINARY );
		cvFindContours( subtracted_frame, g_storage, &contours, 88, CV_CHAIN_APPROX_SIMPLE  );

		cvZero( temp_contours );
		if( contours )
			cvDrawContours(	temp_contours, contours, cvScalarAll(255), cvScalarAll(255), 100, CV_FILLED );

		cvNamedWindow("CONTOURS", CV_WINDOW_KEEPRATIO);
		cvShowImage( "CONTOURS", temp_contours );

		cvMoments(temp_contours, moments, 1);
		// The actual moment values
		double moment10 = cvGetSpatialMoment(moments, 1, 0);
		double moment01 = cvGetSpatialMoment(moments, 0, 1);
		double area = cvGetCentralMoment(moments, 0, 0);
		cout << area;
		if ( area >= slider_min_area ) { // this is to mitigate false positive due to noise
			if (!firstTime){
				position_old = position;
			}
			else {
				firstTime = !firstTime;
				position_old.x = moment10/area;
				position_old.y = moment01/area;
			}

			position.x = moment10/area;
			position.y = moment01/area;
			cvLine(background_plot, position_old, position, cvScalar(255,255,255), 1, CV_AA );

			printf( "position: %d, %d\n", position.x, position.y );
			cvCircle( background_plot, position, 10, cvScalar( 255,255 ), 2, 8, 0 );
			cvCircle( background_plot, position, 1, cvScalar( 255,255 ), 2, 8, 0 );
		} else {
			position = cvPoint( -1, -1 );
		}

		cvResetImageROI(temp_frame_gray);
		cvResetImageROI(temp_background_frame);

		cvReleaseImage(&frame_gray);
		cvReleaseImage(&frame_hsv);
		cvReleaseImage(&subtracted_frame);
		cvReleaseImage(&colour_filtered_frame);


	}
	else {
		//cvDestroyWindow("Subtracted");
		//cvDestroyWindow("CONTOURS");
	}
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
	setMouseCallback( "Main", onMouse, 0 );
}

void loadNext(){
	fileStream.getline(buf, MAX_CHARS_PER_LINE);
	if (buf[0] == '\0') return;
	cvReleaseImage(&file_frame);
	file_frame = cvLoadImage(buf);
	frame_size = cvGetSize(file_frame);
	return;
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

	c = (char) waitKey(80);
	if (c == 'a') bColourFilter = !bColourFilter;
	if (c == 'b') background_subtract_switch_callback(1);

	return 1;
}

void exit_program(){
	destroyAllWindows();
}

CvSize halfCvSize( CvSize size){
	return cvSize(size.width, size.height);
}

bool find_image(Mat needle, Mat haystack){
	//TODO: Find needle in haystack
	return 0;
}
