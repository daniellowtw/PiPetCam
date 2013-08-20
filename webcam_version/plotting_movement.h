//#include "camshift.h"
#include <opencv/cv.h>
#include <opencv2/core/core_c.h>
#include <opencv2/legacy/compat.hpp>

#include <opencv/highgui.h>
#include <opencv2/legacy/legacy.hpp>
//#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
//#include <opencv2/highgui/highgui_c.h>
//#include <opencv/cv.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/video/video.hpp>
//#include <opencv2/video/tracking.hpp>

using namespace std;
using namespace cv;
#define LBPSIZE 36;  //max possible values of LBP
const int MAX_CHARS_PER_LINE = 512;
fstream fileStream;
char buf[MAX_CHARS_PER_LINE];

IplImage* frame; // The frame for processing
IplImage* frame_gray; // Grayscale of frame
IplImage* frame_hsv; // HSV of frame
IplImage* temp_frame; // this is for displaying on Main window
IplImage* temp_frame_hsv; // this is for displaying on Main window
IplImage* background_frame; // Background image for processing
IplImage* background_frame_half; // Backgound image for display in window
IplImage* subtracted_frame; // Image after subtraction (later changed to absDiff)
IplImage* temp_background_frame; // for subtract function
IplImage* temp_frame_gray; // for subtract function
IplImage* temp_contours; // for subtract function
IplImage* background_plot; // for tracing blob
IplImage* colour_filtered_frame; //for filtering by colour
IplImage* image_mask; //for filtering selection
IplImage* file_frame;
IplImage* file_frame_clone; // for drawing rectangles on


int lowerH=0;
int lowerS=0;
int lowerV=0;

int upperH=180;
int upperS=256;
int upperV=256;

bool freeze = false; // controls whether to get image from camera.

int capture_background_switch_value = 0; // For switch acting like a button to capture background
int background_subtract_switch_value = 0; // For switch acting like a button to carry out processing
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)
int slider_min_area = 3000; // Min area threshold. Ideally I want to loop through all the contours to filter small contours.
bool bColourFilter = false;
bool firstTime = true;
CvPoint position; // To store current point value;
CvPoint position_old; // To store current point value;

bool selectObject = false; // Flag for whether user is selecting rect
Rect selection; // Track user rectangular selection TODO: change to polygon shape
IplImage* hull;
CvSize frame_size;
CvSize image_size;

int c;

//Tune
bool backprojMode = false;
int trackObject = 0;
bool showHist = true;
Point origin;
int vmin = 10, vmax = 256, smin = 30;
vector<Point> points;
Point* vertices[4];
int counter=0;

CvCapture* cap;
int fWidth;
int fHeight;

CvSize halfCvSize( CvSize size);