#include <opencv/cv.h>
#include <opencv/highgui.h>

IplImage* subtracted_frame; // Image after subtraction (later changed to absDiff)
IplImage* background_frame_gray; // Grayscale of current frame 
IplImage* frame_gray; // Grayscale of current frame 
IplImage* background_plot; // For drawing the trajectory later
IplImage* temp_frame, *background_frame;
CvCapture* cap;
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)

void setUpWindow(){
	cvNamedWindow("Main", 1);
	cvNamedWindow("Settings", CV_WINDOW_KEEPRATIO);
	cvCreateTrackbar("Threshold", "Settings", &slider_bg_subtract, 255, NULL);
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
}


int main(){
	cap = cvCaptureFromCAM(0);
	assert(cap != NULL); //check if camera is there
	setUpImage(cvSize(cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH), cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT )));
	setUpWindow();

	while (1) {
		temp_frame = cvQueryFrame(cap); // get next frame
		cvShowImage("Main", temp_frame);

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
			case (char) 27:
				return 1;
				break;
			default:
				break;

		}
	}
	return 1;
}