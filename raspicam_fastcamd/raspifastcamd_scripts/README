RaspiFastCamD Scripts
=====================

These scripts in this directory are some helper scripts to control the execution of
raspifastcamd.

The recommended approach is:

1. Start RaspiFastCamD with the script start_camd.sh
   This will create a pid file under /tmp/ and start the daemon in the background.
   (The red camera led will light up). The first parameter is the output filename.
   Be sure to include "%d" in the name, this will be replaced with the number of the
   capture. If ommited, this will default to ~/tmp_%d.jpg
2. Now call do_capture.sh every time you want to capture an image. This will send the
   correct signal to the daemon.
3. When your programm finished call stop_camd.sh
   This will exit the deamon and delete the pid file. Be sure to call this script or
   you'll have to remove the pid at your own.

Please note: These scripts are very basic and just there to show you the general idea.
When you want something more advanced feel free to replace the scripts with your own code.
Especially the do_capture.sh script is much faster when directly embedded to your application.

RaspiFastCamD is not indended as a long running deamon, so don't start it at boot, but just
when your application starts. Starting mulitple instances at the same time will probably mess
up the camera and require a reboot of your pi.
