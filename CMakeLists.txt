cmake_minimum_required(VERSION 2.4.6)

cmake_policy(SET CMP0003 NEW)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

find_package(OpenCV REQUIRED)
find_package(CUDA 8.0 REQUIRED)

include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})

link_directories(${OpenCV_LIBRARY_DIRS})
link_directories(${CUDA_LIBRARY_DIRS})

add_definitions(-DOPENCV -DGPU -DCUDNN)

include_directories(${PROJECT_SOURCE_DIR}/src/darknet)

add_subdirectory(src/darknet)
add_subdirectory(src/main)
