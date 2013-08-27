# Chapter 1: Getting center of pet

1. Get a background image
2. Apply background subtraction to latest frame to get motion layer
3. Apply colour filtering to latest frame to get colour layer
4. Get the intersection of the two layer which we can assume to be our pet
5. Do some sanity check such as getting area of region
6. Find the center of the filtered region 

## Step 1

We first make a way so that we can capture the background image. Due to the inability to create buttons using opencv APIs, we rely on keyboard keys instead. We will assign the space key to capture our background. In our main function:

	#include <opencv/cv.h>
	#include <opencv/highgui.h>
	IplImage* temp_frame, * background_frame;
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

We can compile our code by saving it as `chapter11.c` and run ``gcc chapter11.c -o chapter11 `pkg-config opencv --cflags --libs` `` Note that it is a backtick and not a single quote.

## Step 2

We now need to get the motion layer by applying background subtraction. Background subtraction is a technique wherein a image's forground is extracted for further processing. It is widely used approach for detecting motion. To make processing faster, we convert a colour image into a grayscale and we do some smoothing to remove noises.

To start, we create a function that will be responsible for doing the background subtraction so that we can activate it by pressing a key. We also need a slider to set a threshold value to determine which whether each pixel is different or not.

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
		cvSmooth(frame_gray, frame_gray, CV_GAUSSIAN, 3);
		cvSmooth(background_frame, background_frame_gray, CV_GAUSSIAN, 3);
		cvAbsDiff(frame_gray, background_frame_gray, subtracted_frame);
		cvThreshold( subtracted_frame, subtracted_frame, slider_bg_subtract, 255, CV_THRESH_BINARY );
	}

	void setUpImage(CvSize size){
		frame_gray = cvCreateImage(size,8, 1);
		background_frame_gray = cvCreateImage(size,8, 1);
		subtracted_frame = cvCreateImage(size,8, 1);
	}


In our switch block, we add another case:
	case 'b':
		bg_subtract();
		cvNamedWindow("Subtract", 1);
		cvShowImage("Subtract", subtracted_frame)
		break;

In our main function, we remove the creation of the "Main" window. and add the call to setUpWindow and setUpImage

	setUpImage(cvSize(cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH), cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT )));
	setUpWindow();

## Step 3

Now we create a colour filter layer. Instead of working with RGB, we want to work with HSV instead since it's easier to to think of colours in terms of hue and saturation. We will also create a window that contains a slider bar to choose the range we want and a function to create the colour filter.

	int lowerH=0;
	int lowerS=0;
	int lowerV=0;

	int upperH=180;
	int upperS=256;
	int upperV=256;

	IplImage* colour_filtered_frame; //for filtering by colour
	IplImage* frame_hsv; // HSV of frame

	void setUpWindow(){
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
		cvInRangeS(imgHSV, cvScalar(lowerH,lowerS,lowerV,0), cvScalar(upperH,upperS,upperV,0), imgThresh); 
		return imgThresh;
	}

	void doColourFilter(){
		frame_hsv = cvCreateImage(cvGetSize(temp_frame),8, 3);
		colour_filtered_frame = cvCreateImage(cvGetSize(frame_hsv),8, 1);
		colour_filtered_frame = GetThresholdedImage(frame_hsv);
	}


To have an idea of what area we are filtering, we toggle between normal frame and the filtered frame. First declare the flag

	int bColourFilter = 0;

In our switch block, we add another case:
	case 'a':
		if (bColourFilter == 1) bColourFilter = 0;
		else bColourFilter = 1;
		break;

Now, in our main loop, we check if the flag is 1 or 0, and show the respective frame

	whiel (1){
		...
		cvCvtColor(temp_frame, frame_hsv, CV_RGB2HSV);
		if (bColourFilter) {
			cvShowImage("Main", GetThresholdedImage(frame_hsv));
		}
		else {
			cvShowImage("Main", temp_frame);
		}
	}

Lastly, we need to call the function doColourFilter. 

	case 'b':
		...
		doColourFilter();
		break;

If we run our program now, and press the key 'a', we should see a white screen, and if we toggle the HSV sliders, we will see that only some of the white region remains. Those are the regions that are within the HSV range that we specified.


## Step 4, 5, 6

We now combine both layer and find the contour which we can use to determine other information such as area and center. We put this in a function so that we can determine when to do processing.

	void doProcessing(){
		cvAnd(subtracted_frame, colour_filtered_frame, subtracted_frame, NULL);
		CvMemStorage*  g_storage  = NULL;
		CvSeq* contours = 0;
		CvMoments *moments = (CvMoments*) malloc(sizeof(CvMoments));

		g_storage = cvCreateMemStorage(0);
		cvFindContours( subtracted_frame, g_storage, &contours, 88, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
		cvMoments(temp_contours, moments, 1);
		cvZero( temp_contours );
		if( contours )
			cvDrawContours(	temp_contours, contours, cvScalarAll(255), cvScalarAll(255), 100, CV_FILLED );

		// The actual moment values
		double moment10 = cvGetSpatialMoment(moments, 1, 0); // This is our x value
		double moment01 = cvGetSpatialMoment(moments, 0, 1); // This is our y value
		double area = cvGetCentralMoment(moments, 0, 0); // This is our area
	}

We add another sliders to determine the minimum area for there to be motion. If the area is more than the minimum, we will plot the point. But before we can do that, we need to create another image to store our plot. In our bg_subtract function, we shall clone the background image.

Add the following to setUpWindow

	cvCreateTrackbar("Min Area", "Settings", &slider_min_area, 100000, NULL);

Add the following after capturing a background

	background_plot = cvCloneImage(background_frame); // Set background plot to draw on image
	

	IplImage* temp_contours; // for subtract function
	int slider_min_area = 3000; // Min area threshold. Ideally I want to loop through all the contours to filter small contours.
	CvPoint position; // To store current point value;
	CvPoint position_old; // To store current point value;
	int firstTime = 1;

	void doProcessing() {
		...
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

Now add that to our processing call:

	case 'b':
			...
			doProcessing();
			break;