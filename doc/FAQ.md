FAQ
===

Q: I am using uv4l driver, but I can only get 64 x 64 resolution when I use opencv

A: Use the cpp API instead of the c API. Also include the following flags when compiling
`-L/usr/lib/uv4l/uv4lext/armv6l -luv4lext -Wl,-rpath,'/usr/lib/uv4l/uv4lext/armv6l'`

Q: Can I use the raspiberry pi camera with opencv?

A: Yes, there is a driver available. Look at 
http://www.linux-projects.org/modules/sections/index.php?op=viewarticle&artid=16
http://www.linux-projects.org/modules/sections/index.php?op=viewarticle&artid=15
http://www.linux-projects.org/downloads/examples/opencv_test.cpp