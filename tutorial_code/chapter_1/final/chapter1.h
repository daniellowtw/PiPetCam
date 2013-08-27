#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat temp_frame, background_frame, frame_gray, background_frame_gray, subtracted_frame;
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)

int lowerH=0;
int lowerS=0;
int lowerV=0;

int upperH=180;
int upperS=256;
int upperV=256;

Mat colour_filtered_frame; //for filtering by colour
Mat frame_hsv; // HSV of frame
Mat background_plot;
bool bColourFilter = false;

bool firstTime = true;
int slider_min_area = 3000; // Min area threshold. Ideally I want to loop through all the contours to filter small contours.
Point position; // To store current point value;
Point position_old; // To store current point value;

