# Introduction and Setup

![PiPetCam][picture of pipetcam]

Welcome to PiPetCam, in this project you will create a pet monitoring system by doing some image processing[^1] and image analysis[^2]. The idea is to collect image data from the pi camera and extract information such as the center of the pet. With that data, it is possible to do things like plotting a movement map, a heat map, and event trigger. 

## Features

This set of tutorials will guide you through some image manipulation and some web development tools. It is targeted at people who are interested in finding out what their pets are up to and also anyone looking to have some fun with image processing.

We will be using OpenCV library which is a library mainly aimed at image processing and computer vision.

If you are new to computer graphics, reading the tutorial from last year is quite useful.

In particular, you should understand the following:

* [Colour space][colour space]
* [Introduction to c][c tutorial]

# Goals and what you will learn

* Create a pet mintoring system for your pet
* Learn some image processing concepts
* Learn to use some OpenCV library (helpful for future projects.
* Learn some javascript

## Colour space
## Image processing
## Node js for remote controlling
## Socket to communicate between web and program


# Quick overview of project

## [Chapter 1: Getting center of pet][chapter 1]

1. Get a background image
2. Apply background subtraction to latest frame to get motion layer
3. Apply colour filtering to latest frame to get colour layer
4. Do some sanity check such as getting area of region
5. Get the intersection of the two layer which we can assume to be our pet
6. Find the center of the filtered region 

## [Chapter 2: Setting up event triggers for pet][chapter 2]

1. Use the center to determine if pet has entered certain region
2. Set up triggers for that event

## [Chapter 3: Setting up a server and display data][chapter 3]

1. Set up a listening server
2. Set up listeners to events
3. Create scripts to execute when events happen
4. Set up data visualising apps

## [Chapter 4: Setting up streaming][chapter 4]

1. Get necessary files and install it
2. Write a script to start streaming


# Installation

1. Installation instruction and files for rasbian can be found [here][rasbian_download]
2. Install OpenCV library by typing `sudo apt-get install libopencv-dev` Test the installation by typing `pkg-config opencv --modversion`
3. Type `sudo apt-get install lighttpd

# Let's get started

Click <a href = "chapter1-datatracking.html">here</a> to get started with the first chapter.

[chapter1]:/chapter1-datatracking.html
[rasbian_download]:http://www.raspberrypi.org/downloads
[colour space]:http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/image-processing/intro.html#colour
[c tutorial]:http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/rpii/cintro.html
[picture of pipetcam]:http://placehold.it/350x350
[^1]:Image processing is a method to convert an image into digital form and perform some operations on it, in order to get an enhanced image or to extract some useful information from it. 
[^2]:Image analysis is the extraction of meaningful information from an image. 


[chapter 1]:/twdl3/writeup/chapter1-datatracking.html
[chapter 2]:/twdl3/writeup/chapter2-region_events.html
[chapter 3]:/twdl3/writeup/chapter3-making_an_interface.html
[chapter 4]:/twdl3/writeup/chapter3-making_an_interface.html