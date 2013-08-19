PiPetCam
========

## Requirements

1. Working raspi running **2013-05-25-wheezy-raspbian** with camera module enabled
2. OpenCV library (using 2.3.1 for this project)

## Installation

```
mkdir build
cd build
cmake ..
make
./pet
```

## Usage

* c and b toggles preview overlay
* Esc to quit
* 'TEST' window shows the opencv frames

## TODO

* Get coloured image for opencv function

# Optional tweaks

1. Disable the red light by adding `disable_camera_led=1` to /boot/config.txt by typing `sudo nano /boot/config.txt` and rebooting the pi.