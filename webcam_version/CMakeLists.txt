cmake_minimum_required(VERSION 2.8)

SET(COMPILE_DEFINITIONS -Werror)

add_executable(pet_webcam plotting_movement.cpp )

find_package( OpenCV REQUIRED )

target_link_libraries(pet_webcam ${OpenCV_LIBS})
