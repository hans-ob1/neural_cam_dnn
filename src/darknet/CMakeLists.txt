cmake_minimum_required(VERSION 2.4.6)

cmake_policy(SET CMP0003 NEW)

project(darknet_lib C CXX)

file(GLOB DARKNET_SOURCES "*.c" "*.cu" "*.h")

set(DARKNET_LIBRARIES m pthread stdc++ cublas curand cudnn)

cuda_add_library(${PROJECT_NAME} STATIC ${DARKNET_SOURCES} OPTIONS -arch sm_61)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${DARKNET_LIBRARIES})




