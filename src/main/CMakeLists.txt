cmake_minimum_required(VERSION 2.4.6)

cmake_policy(SET CMP0003 NEW)

project(neural_cam_dnn C CXX)

set(LIBRARIES
    m
    pthread
    stdc++
    ${OpenCV_LIBS}
    darknet_lib
)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )
cuda_add_library(supportlib supportfunc.cpp supportfunc.hpp)
cuda_add_executable(${PROJECT_NAME} main.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBRARIES} supportlib)



