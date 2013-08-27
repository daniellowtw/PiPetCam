#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage* subtracted_frame; // Image after subtraction (later changed to absDiff)
IplImage* background_frame_gray; // Grayscale of current frame 
IplImage* frame_gray; // Grayscale of current frame 
IplImage* background_plot; // For drawing the trajectory later
IplImage* temp_frame, *background_frame;
IplImage* temp_contours; // for subtract function


CvCapture* cap;
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)


int lowerH=0;
int lowerS=0;
int lowerV=0;

int upperH=180;
int upperS=256;
int upperV=256;

IplImage* colour_filtered_frame; //for filtering by colour
IplImage* frame_hsv; // HSV of frame
int bColourFilter = 0;
int firstTime = 1;

int slider_min_area = 3000; // Min area threshold. Ideally I want to loop through all the contours to filter small contours.
CvPoint position; // To store current point value;
CvPoint position_old; // To store current point value;
CvPoint origin;

CvRect selection; // Track user rectangular selection
CvRect temp_selection; // Track user rectangular selection
int selectObject = 1; // Flag for whether user is selecting rect

int abs(int n) { return n > 0 ? n : -n; }

static void onMouse( int event, int x, int y, int foo, void* bar) {
	if( selectObject ) {
		temp_selection.x = MIN(x, origin.x);
		temp_selection.y = MIN(y, origin.y);
		temp_selection.width = abs(x - origin.x);
		temp_selection.height = abs(y - origin.y);
	}
	switch( event ) {
	case CV_EVENT_LBUTTONDOWN:
		origin = cvPoint(x,y);
		temp_selection = cvRect(x,y,0,0);
		selectObject = 1;
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = 0;
		selection = temp_selection; // Saves our selection
		temp_selection = cvRect(0,0,0,0); // hides our temp selection
		break;
	}
}


void drawRectangles(IplImage* frame, CvRect selection, int colour_choice){ 
	CvPoint p1,p2;
	CvScalar temp_colour;
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
	cvRectangle(frame,p1,p2, temp_colour,1,CV_AA,0);
}

void setUpWindow(){
	cvNamedWindow("Settings", CV_WINDOW_KEEPRATIO);
	cvCreateTrackbar("Threshold", "Settings", &slider_bg_subtract, 255, NULL);
	cvCreateTrackbar("LowerH", "Settings", &lowerH, 180, NULL);
	cvCreateTrackbar("UpperH", "Settings", &upperH, 180, NULL);
	cvCreateTrackbar("LowerS", "Settings", &lowerS, 256, NULL);
	cvCreateTrackbar("UpperS", "Settings", &upperS, 256, NULL);
	cvCreateTrackbar("LowerV", "Settings", &lowerV, 256, NULL);
	cvCreateTrackbar("UpperV", "Settings", &upperV, 256, NULL); 
	cvSetMouseCallback( "Main", onMouse, 0 );

}

IplImage* GetThresholdedImage(IplImage* imgHSV){
	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV, 0), cvScalar(upperH,upperS,upperV, 0), imgThresh); 
	return imgThresh;
}

void doColourFilter(){
	frame_hsv = cvCreateImage(cvGetSize(temp_frame),8, 3);
	colour_filtered_frame = cvCreateImage(cvGetSize(frame_hsv),8, 1);
	colour_filtered_frame = GetThresholdedImage(frame_hsv);
}

void bg_subtract() {
	cvCvtColor(temp_frame, frame_gray, CV_BGR2GRAY);
	cvCvtColor(background_frame, background_frame_gray, CV_BGR2GRAY);
	cvSmooth(frame_gray, frame_gray, CV_GAUSSIAN, 3, 0, 0, 0);
	cvSmooth(background_frame, background_frame_gray, CV_GAUSSIAN, 3, 0, 0, 0);
	cvAbsDiff(frame_gray, background_frame_gray, subtracted_frame);
	cvThreshold( subtracted_frame, subtracted_frame, slider_bg_subtract, 255, CV_THRESH_BINARY );
	background_plot = cvCloneImage(background_frame); // Set background plot to draw on image
}

void doProcessing(){

	cvAnd(subtracted_frame, colour_filtered_frame, subtracted_frame, NULL);
	CvMemStorage*  g_storage  = NULL;
	CvSeq* contours = 0;
	CvMoments *moments = (CvMoments*) malloc(sizeof(CvMoments));

	g_storage = cvCreateMemStorage(0);
	cvFindContours( subtracted_frame, g_storage, &contours, 88, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	cvMoments(temp_contours, moments, 1);

	// The actual moment values
	double moment10 = cvGetSpatialMoment(moments, 1, 0); // This is our x value
	double moment01 = cvGetSpatialMoment(moments, 0, 1); // This is our y value
	double area = cvGetCentralMoment(moments, 0, 0); // This is our area

	if ( area >= slider_min_area ) { // this is to mitigate false positive due to noise
		if (firstTime == 0){
			position_old = position;
		}
		else {
			firstTime = 0;
			position_old.x = moment10/area;
			position_old.y = moment01/area;
		}

		position.x = moment10/area;
		position.y = moment01/area;
		cvLine(background_plot, position_old, position, cvScalar(255,255,255,0), 1, CV_AA, 0);

		printf( "position: %d, %d\n", position.x, position.y );
		cvCircle( background_plot, position, 10, cvScalar( 255,255, 0, 0 ), 2, 8, 0 );
		cvCircle( background_plot, position, 1, cvScalar( 255,255, 0, 0 ), 2, 8, 0 );
	} else {
		position = cvPoint( -1, -1 );
		firstTime = 1;
	}
}

int main(){

	cap = cvCaptureFromCAM(0);
	// assert(cap != NULL); //check if camera is there
	cvNamedWindow("Main", 1);

	while (1) {
		temp_frame = cvQueryFrame(cap); // get next frame
		cvCvtColor(temp_frame, frame_hsv, CV_RGB2HSV);
		drawRectangles(temp_frame, temp_selection, 0); //required for drawing user's selection
		drawRectangles(temp_frame, selection , 1);

		if (bColourFilter) {
			cvShowImage("Main", GetThresholdedImage(frame_hsv));
		}
		else {
			cvShowImage("Main", temp_frame);
		}

		switch ((char) cvWaitKey(30)){
			case ' ':
				background_frame = cvCloneImage(temp_frame); // Copy frame
				cvNamedWindow("Background_plot", 0);
				cvShowImage("Background_plot", background_plot);
				break;
			case 'b':
				bg_subtract();
				doColourFilter();
				doProcessing();
				break;

			case 'a':
				if (bColourFilter == 1) bColourFilter = 0;
				else bColourFilter = 1;
				break;
			default:
				break;
		}
	}
	return 1;
}