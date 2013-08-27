#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage* subtracted_frame; // Image after subtraction (later changed to absDiff)
IplImage* background_frame_gray; // Grayscale of current frame 
IplImage* frame_gray; // Grayscale of current frame 
IplImage* background_plot; // For drawing the trajectory later
IplImage* temp_frame, *background_frame;
CvCapture* cap;
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)

IplImage* temp_contours; // for subtract function

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

void setUpWindow(){
	cvNamedWindow("Main", 1);
	cvNamedWindow("Settings", CV_WINDOW_KEEPRATIO);
	cvCreateTrackbar("Threshold", "Settings", &slider_bg_subtract, 255, NULL);
	cvCreateTrackbar("Min Area", "Settings", &slider_min_area, 100000, NULL);
	cvCreateTrackbar("LowerH", "Settings", &lowerH, 180, NULL);
	cvCreateTrackbar("UpperH", "Settings", &upperH, 180, NULL);
	cvCreateTrackbar("LowerS", "Settings", &lowerS, 256, NULL);
	cvCreateTrackbar("UpperS", "Settings", &upperS, 256, NULL);
	cvCreateTrackbar("LowerV", "Settings", &lowerV, 256, NULL);
	cvCreateTrackbar("UpperV", "Settings", &upperV, 256, NULL); 
}

IplImage* GetThresholdedImage(IplImage* imgHSV){
	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
	cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV, 0), cvScalar(upperH,upperS,upperV, 0), imgThresh); 
	return imgThresh;
}

void doColourFilter(){
	colour_filtered_frame = cvCreateImage(cvGetSize(frame_hsv),8, 1);
	colour_filtered_frame = GetThresholdedImage(frame_hsv);
}

void bg_subtract() {
	cvCvtColor(temp_frame, frame_gray, CV_BGR2GRAY);
	cvCvtColor(background_frame, background_frame_gray, CV_BGR2GRAY);
	cvSmooth(frame_gray, frame_gray, CV_GAUSSIAN, 3, 3, 0, 0);
	cvSmooth(background_frame_gray, background_frame_gray, CV_GAUSSIAN, 3, 3, 0, 0);
	cvAbsDiff(frame_gray, background_frame_gray, subtracted_frame);
	cvThreshold( subtracted_frame, subtracted_frame, slider_bg_subtract, 255, CV_THRESH_BINARY );
}

void setUpImage(CvSize size){
	frame_gray = cvCreateImage(size,8, 1);
	background_frame_gray = cvCreateImage(size,8, 1);
	subtracted_frame = cvCreateImage(size,8, 1);
	// background_plot = cvCreateImage(size,8, 1);
	frame_hsv = cvCreateImage(size,8, 3);
	temp_contours = cvCreateImage(cvGetSize(frame_gray),8, 1);

}

void doProcessing(){

	cvAnd(subtracted_frame, colour_filtered_frame, subtracted_frame, NULL);
	CvMemStorage*  g_storage  = NULL;
	CvSeq* contours = 0;
	CvMoments *moments = (CvMoments*) malloc(sizeof(CvMoments));

	g_storage = cvCreateMemStorage(0);
	cvFindContours( subtracted_frame, g_storage, &contours, 88, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	cvZero( temp_contours ); // create a black image to draw our contours on
	if( contours )
		cvDrawContours(	temp_contours, contours, cvScalarAll(255), cvScalarAll(255), 100, 1, 8, cvPoint(0,0));

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
	assert(cap != NULL); //check if camera is there
		temp_frame = cvQueryFrame(cap); // get next frame

	printf("%lu %lu\n", cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH), cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT));
	// cvSetCaptureProperty(cap,CV_CAP_PROP_FRAME_WIDTH, 320);
	// cvSetCaptureProperty(cap,CV_CAP_PROP_FRAME_HEIGHT, 240);
	cvNamedWindow("Main", 0);
	setUpImage(cvSize(cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH), cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT )));	
	// setUpImage(cvSize(64,64));	

	setUpWindow();

	while (1) {
		temp_frame = cvQueryFrame(cap); // get next frame
		cvCvtColor(temp_frame, frame_hsv, CV_RGB2HSV);

		if (bColourFilter) {
			cvShowImage("Main", GetThresholdedImage(frame_hsv));
		}
		else {
			cvShowImage("Main", temp_frame);
		}

		switch ((char) cvWaitKey(30)){
			case ' ':
				background_frame = cvCloneImage(temp_frame); // Copy frame
				cvNamedWindow("Background", 1);
				cvShowImage("Background", background_frame);
				background_plot = cvCloneImage(background_frame); // Set background plot to draw on image
				break;
			case 'b':
				bg_subtract();
				doColourFilter();
				doProcessing();
				cvNamedWindow("Subtract", 1);
				cvShowImage("Subtract", subtracted_frame);
				cvNamedWindow("Plot", 1);
				cvShowImage("Plot", background_plot);
				break;
			case 'a':
				if (bColourFilter == 1) bColourFilter = 0;
				else bColourFilter = 1;
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