#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat temp_frame, background_frame;

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
	namedWindow("Main", 1);

	while (1)
	{
		cap.read(temp_frame);
		imshow("Main", temp_frame);

		switch ((char) waitKey(30))
		{
		case ' ':
			background_frame = Mat(temp_frame); // Copy frame
			namedWindow("Background", 1);
			imshow("Background", background_frame);
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