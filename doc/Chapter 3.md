# Streaming video from the pi


read http://www.raspberrypi.org/phpBB3/viewtopic.php?t=45368

Problem is with playing back on devices since it uses flash

We need to set up a crtmspserver 

`sudo apt-get install crtmspserver`


raspivid -t 9999999 -w 960 -h 540 -fps 25 -b 500000 -vf -o - | ffmpeg -i - -vcodec copy -an -r 25 -f flv -metadata streamName=myStream tcp://0.0.0.0:6666