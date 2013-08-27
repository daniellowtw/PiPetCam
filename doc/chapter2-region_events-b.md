# Chapter 2: triggering events in region

Apart from collecting data, we want our system to be slightly more intelligent. We would like for it to know if our pet has entered a certain region. There are two ways of doing this. In the first way, we can use the data we collected in chapter 1, namely the center of the pet and test if it has entered a region we defined. The other way is to define a region in our frame and test for motion only in that frame. This chapter will guide you through both methods.

## Setting it up: Letting user select region

Before we can continue, we need to get a region to track. This can be done by creating a mouse listener.

	CvPoint origin;
	CvRect selection; // Track user rectangular selection
	CvRect temp_selection; // Track user rectangular selection
	int selectObject = 1; // Flag for whether user is selecting rect


	int abs(int n) { return n > 0 ? n : -n; }

	static void onMouse( int event, int x, int y, int foo, void* bar) {
		if( selectObject ) {
			temp_selection.x = MIN(x, origin.x);
			temp_selection.y = MIN(y, origin.y);
			temp_selection.width = abs(x - origin.x);
			temp_selection.height = abs(y - origin.y);
		}
		switch( event ) {
		case CV_EVENT_LBUTTONDOWN:
			origin = cvPoint(x,y);
			temp_selection = cvRect(x,y,0,0);
			selectObject = 1;
			break;
		case CV_EVENT_LBUTTONUP:
			selectObject = 0;
			selection = temp_selection; // Saves our selection
			temp_selection = cvRect(0,0,0,0); // hides our temp selection
			break;
		}
	}

To give the user a feedback on selection, we draw a rectangle when user is dragging the screen

	void drawRectangles(IplImage* frame, CvRect selection, int colour_choice){
		CvPoint p1,p2;
		CvScalar temp_colour;
		p1.x = selection.x;
		p2.x = selection.x + selection.width;
		p1.y = selection.y;
		p2.y = selection.y + selection.height;

		switch (colour_choice)
		{
		case 1:
			temp_colour = CV_RGB(0,255,0);
			break;
		case 2:
			temp_colour = CV_RGB(0,0,255);
			break;
		default:
			temp_colour = CV_RGB(255,0,0);
			break;
		}
		cvRectangle(frame,p1,p2, temp_colour,1,CV_AA,0);
	}

Now we attach it to our "Main" window

	void setUpWindow(){
		...
		cvSetMouseCallback( "Main", onMouse, 0 );
	}

And in our while loop we will make it draw the user selection

	while(1) {
		...
		drawRectangles(temp_frame, temp_selection, 0); //required for drawing user's selection
		drawRectangles(temp_frame, selection , 1);

	}

We should also make our background plot appear when a picture is taken, so in our switch block, we make the window appear when we press space.

	case ' ':
		...
		cvNamedWindow("Background_plot", 0);
		cvShowImage("Background_plot", background_plot);


## Method 1: Coordinates of pet center

Now that we have the user's selection stored in the selection variable, we just need to check whether the pet's center is in the rectangle. We can do this in our doProcessing function.

	void doProcessing(){
		...
		if (moment10 > selection.x && moment10 < selection.x + selection.width && moment01 > selection.y && moment01 < selection.y + selection.height) {
			cvSaveImage("event_triggered.jpg", temp_frame);
		}
	}

## Method 2: Detect motion in a certain region only

Instead of simply testing a coordinates, we can also do the whole processing again, but only for a region. The idea here is to limit the region of interest (ROI) to the user's selection. So that processing is only done in that ROI.
