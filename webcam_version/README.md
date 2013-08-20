PiPetCam (webcam)
========

## Requirements

1. Working raspi running **2013-05-25-wheezy-raspbian** with a USB webcam
2. OpenCV library (using 2.3.1 for this project)

## Installation

```
mkdir build
cd build
cmake ..
make
./pet_webcam
```

## Usage

1) Click Cap BG to capture background
2) Drag a region to monitor for changes in the 'Main' window
3) Press BG Subgract to detect motion.

* Press b to trigger background subtract after the first time.
* Threshold determines sensitivity to changes between frame and background
* Min Area determines the minimum area of differences before the center is calcluated and recorded.
* HSV sliders to filter by colour
* Esc to quit

## TODO

* Store center of change in a file for future processing

## FAQ

Q: Program crashes when I click background subtract
A: Have you made sure you have a selected a region?

Q: Nothing happens when I press b
A: Try toggling it manually first