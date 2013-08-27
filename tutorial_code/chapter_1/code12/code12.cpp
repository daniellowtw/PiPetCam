#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat temp_frame, background_frame, frame_gray, background_frame_gray, subtracted_frame;
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)

void setUpWindow(){
	namedWindow("Main", 1);
	namedWindow("Settings", CV_WINDOW_KEEPRATIO);
	createTrackbar("Threshold", "Settings", &slider_bg_subtract, 255, NULL);
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
		imshow("Main", temp_frame);

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