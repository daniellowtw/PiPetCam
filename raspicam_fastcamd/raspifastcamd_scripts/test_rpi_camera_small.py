import time
from PIL import Image
import ImageFilter
import cStringIO
import subprocess
import cv2
import numpy as np


def create_opencv_image_from_stringio(img_stream, cv2_img_flag=0):
    img_stream.seek(0)
    img_array = np.asarray(bytearray(img_stream.read()), dtype=np.uint8)
    return cv2.imdecode(img_array, cv2_img_flag)


def create_opencv_image_from_url(url, cv2_img_flag=0):
    request = urlopen(url)
    img_array = np.asarray(bytearray(request.read()), dtype=np.uint8)
    return cv2.imdecode(img_array, cv2_img_flag)


def capture_test_image():
    command = "./do_capture.sh"
    subprocess.call(["/opt/vc/userland/host_applications/linux/apps/fastcam/raspifastcamd_scripts/do_capture.sh"],shell=True)
    return 1
    # StringIO used here as to not wear out the SD card
    # There will be a lot of these pics taken
    imageData = cStringIO.StringIO()
    imageData.write(subprocess.check_output(command, shell=True))
    imageData.seek(0)
    test = create_opencv_image_from_stringio(imageData)
    # im = Image.open(imageData)
##    buffer = Image.open(imageData)
    # buffer = im.load()
##    return buffer, imageData
    return test





time1 = time.time()

capture_test_image();
##print type(buff)
##buff = buff.filter(ImageFilter.BLUR)
##buff.save("test1.jpg");
test=capture_test_image()
print type(test)

time2 = time.time()
capture_test_image();
capture_test_image();
capture_test_image();
capture_test_image();

#subprocess.call (["raspistill -t 0 -o /run/shm/b.jpg"], shell = True)

time3 = time.time()
capture_test_image();
capture_test_image();
capture_test_image();
capture_test_image();
capture_test_image();
capture_test_image();
capture_test_image();
capture_test_image();

#subprocess.call (["raspistill -t 0 -o test.jpg"], shell = True)

time4 = time.time()

print time2 - time1;
print time3 - time2;
print time4 - time3;
