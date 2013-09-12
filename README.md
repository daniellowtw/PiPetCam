PiPetCam
========

A pet monitoring project using raspberry pi

## What is it

This project aims to use the raspiberry pi as a pet monitoring system.

## Use cases

* Watching over food bowl
* Tracking movements
* Pet surveillance camera
* Pet triggered events

### Watching over food bowl

The system can be used to oversee the area where the pet usually has its meal and can be triggered to take a picture when the pet is in the area. The picture can be stored externally on an attached storage or internally and viewed via a web interface.

Data such as how many times the pet has drank from the waterbowl which can then be used as an indicator of when the water needs refilling.

### Tracking movements 

The system can be deployed to oversee enclosed pets such as hamster, bird, fishes etc. By comparing the current frame with a predetermined background picture, movements can be tracked and plotted. The data can be visualised using a heatmap which can be viewed via a web interface.

Data collected can reveal where the pet likes to spend its time. 

### Pet surveillance camera

The camera captures an area where the pet likes to be. It streams the video over the network and it can be viewed by a browser, or devices. This can be modified to act like a internet protocol camera (IP camera) which allows owners to monitor their pet when they are away.

### Pet triggered events

A region of interest (ROI) can be designated so that movements in the ROI can be used to trigger events such as taking a picture/twitting.

## Requirements

1. Working raspi running **2013-05-25-wheezy-raspbian** with camera module enabled
2. OpenCV library (using 2.3.1 for this project)
3. WebServer?
4. SSH, VNC, Synergy set up (optional)


## Installation

1. Installation instruction and files for rasbian can be found [here][rasbian_download]
2. Install OpenCV library by typing `sudo apt-get install libopencv-dev` Test the installation by typing `pkg-config opencv --modversion`
3. Type `sudo apt-get install lighttpd`

## In this project

* Camera_version contains files that uses the buffer from the camera video port to feed to opencv
* daemon_version contains files that show how to use signals to communicate with the program so that it can run in daemon mode
* data_visualisation contains the files that ideally visualises the output we have
* doc contains general documentations and tutorials
* streaming contains instructions on how to get the pi to stream video over a network
* tutorial code contains code to guide users through some simple image processing basics
* webcam_version contains c++ code for generic use with a webcam, but works on the pi as well

# Other Documentations

* [Raspi cam documentation][raspicam_doc]
* [OpenCv][opencv_doc]


[rasbian_download]:http://www.raspberrypi.org/downloads
[raspicam_doc]:http://www.raspberrypi.org/wp-content/uploads/2013/07/RaspiCam-Documentation.pdf
[opencv_doc]:http://docs.opencv.org

# Optional tweaks

1. Disable the red light by adding `disable_camera_led=1` to /boot/config.txt by typing `sudo nano /boot/config.txt` and rebooting the pi.

# TODO

* Create socket so that program can be controlled via nodejs
* Explore daemon mode to invoke capture via signals instead of a script file.
* Complete the camera version (ie without using any drivers)
