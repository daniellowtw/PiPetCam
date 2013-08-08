#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

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

int lowerH=0;
int lowerS=0;
int lowerV=0;

int upperH=180;
int upperS=256;
int upperV=256;

int freeze = 0; // controls whether to get image from camera.

int capture_background_switch_value = 0; // For switch acting like a button to capture background
int background_subtract_switch_value = 0; // For switch acting like a button to carry out processing
int slider_bg_subtract = 50; // background subtract treshold (controlling binary threshold)
int slider_min_area = 3000; // Min area threshold. Ideally I want to loop through all the contours to filter small contours.
int bColourFilter = 0;
int firstTime = 1;
CvPoint position; // To store current point value;
CvPoint position_old; // To store current point value;

int selectObject = 0; // Flag for whether user is selecting rect
CvRect selection; // Track user rectangular selection TODO: change to polygon shape
IplImage* hull;
CvSize frame_size;
CvSize image_size;

int c;

//Tune
int backprojMode = 0;
int trackObject = 0;
int showHist = 1;
CvPoint origin;
int vmin = 10, vmax = 256, smin = 30;
int counter=0;

CvCapture* cap;
int fWidth;
int fHeight;

CvSize halfCvSize( CvSize size);

int plotting_movement(IplImage* cam_frame);
void setUpWindow();
void exit_program();
void print_stuff();