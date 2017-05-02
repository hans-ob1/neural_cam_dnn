cmake_minimum_required(VERSION 2.4.6)

cmake_policy(SET CMP0003 NEW)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

find_package(CUDA 8.0 REQUIRED)
find_package(OpenCV 3.1 REQUIRED)

INCLUDE(FindCUDA)
INCLUDE_DIRECTORIES(/usr/local/cuda/include)


set(
    CUDA_NVCC_FLAGS
    ${CUDA_NVCC_FLAGS};
    -O3 -gencode arch=compute_52,code=sm_52
)


add_definitions(-DGPU -DCUDNN)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c" "src/*.cu" "src/*.h")

cuda_add_library(darknetlib ${SOURCES} OPTIONS -arch sm_52)
TARGET_LINK_LIBRARIES(darknetlib m pthread stdc++ cublas curand cudnn)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )

cuda_add_library(supportlib src/supportfunc.cpp src/supportfunc.hpp)
cuda_add_executable(main src/main.cpp)
TARGET_LINK_LIBRARIES(main m pthread stdc++ darknetlib ${OpenCV_LIBS} supportlib)




