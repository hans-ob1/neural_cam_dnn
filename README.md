[![Travis](https://img.shields.io/travis/rust-lang/rust.svg)]()
[![Codecov](https://img.shields.io/codecov/c/github/codecov/example-python.svg)]()
# neural_cam_dnn
neural_cam_dnn turns your USB camera into object detector that detect or classify objects it seen before. The backend framework used is darknet developed by Joseph Redmon. This version is a fork, modified to streamline training process and compiled with cmake. Feel free to modify to your own use with no warranty.

##### Current Feature:
- training/evaluate yolo detector

##### Usage Details:
- the compiled program would parse the content of setup.cfg file. Be sure to modify/fill up the content based on the model format given. Typically, if you want to train your own model, you need to include your own architecture, location of the training data, evaluation data if you have any. 

##### Usage Steps:
- (from the directory you installed)
- mkdir build
- cd build and cmake ..
- make
- ./main train (for training the detector, use darknet19_448.conv.23 in onroad_cfg/full_model_3class folder)
- ./main eval <device number> (for evalution of the detector, e.g: "./main eval 0" means accessing "/dev/video0")

##### Changes: (from v1.7 onwards)
- There are some issues with 16.04 with ROS installed concurrently. The problem exists float point exception
- Opencv component is installed into image.c file, provide a more convenient way for loading images
- Since Opencv function is used in the core libraries, Opencv 3.1 is required for cmake
- Mat is fused with image struct instead of Ipllimage
- setup.cfg is used for loading of configuration, just have to modify this file base on where you place your files

##### Requirements:
- cmake 2.8 above
- runs on opencv 3.1
- ubuntu 14.04 above
- cuda 7.5 above (modify the CMakeList.txt if you dont have a GPU!)
- nvidia graphic driver 367.48
