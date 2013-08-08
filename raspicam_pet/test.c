#include <opencv/cv.h>
#include <opencv/highgui.h>
// #include <opencv2/core/core_c.h>

#include <stdio.h>

int main(){
	CvPoint point = cvPoint(100,100);
	CvSize testsize = cvGetSize(cam_frame);
	fprintf(stderr, "%d", point.x);
}