#include <opencv/cv.h>
#include <opencv/highgui.h>
IplImage* temp_frame, *background_frame;
CvCapture* cap;

int main(){

	cap = cvCaptureFromCAM(0);
	assert(cap != NULL); //check if camera is there
	cvNamedWindow("Main", 1);

	while (1) {
		temp_frame = cvQueryFrame(cap); // get next frame
		cvShowImage("Main", temp_frame);

		switch ((char) cvWaitKey(30)){
			case ' ':
				background_frame = cvCloneImage(temp_frame); // Copy frame
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