#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage* subtracted_frame; // Image after subtraction (later changed to absDiff)
IplImage* background_frame_gray; // Grayscale of current frame 
IplImage* frame_gray; // Grayscale of current frame 
IplImage* background_plot; // For drawing the trajectory later
IplImage* temp_frame, *background_frame;
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


void setUpWindow(){
	cvNamedWindow("Main", 1);
	cvNamedWindow("Settings", CV_WINDOW_KEEPRATIO);
	cvCreateTrackbar("Threshold", "Settings", &slider_bg_subtract, 255, NULL);
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
}

int main(){
	cap = cvCaptureFromCAM(0);
	assert(cap != NULL); //check if camera is there
	cvNamedWindow("Main", 0);
	setUpImage(cvSize(cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH), cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT )));	
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
				break;
			case 'b':
				bg_subtract();
				cvNamedWindow("Subtract", 1);
				cvShowImage("Subtract", subtracted_frame);
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