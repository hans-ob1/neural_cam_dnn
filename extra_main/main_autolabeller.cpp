//auto labeller


#include <iostream>
#include <stdlib.h>     /* malloc, calloc, realloc, free */

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/videoio/videoio.hpp>  // Video write
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>

#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
using namespace cv;


const int human_train_num = 5000;
const int human_test_num = 1000; 

int train_pos_counter = 0;
int train_neg_counter = 0;

int test_pos_counter = 0;
int test_neg_counter = 0;


extern "C" {
#include "detector.h"
}

// ****************************>>>>>>>>>>>>>>
// ********* support functions >>>>>>>>>>>>>>
// ****************************>>>>>>>>>>>>>>

//camera stream
CvCapture *cap = cvCaptureFromCAM(CV_CAP_ANY);
IplImage* src;
Mat img_cpp;

int framecounter = 0;

bool human_flag = false;

//parallel thread for camera streaming
void camera_stream(){

    cvSetCaptureProperty(cap,CV_CAP_PROP_FRAME_WIDTH,640);
    cvSetCaptureProperty(cap,CV_CAP_PROP_FRAME_HEIGHT,480);

    while(1){
       src = cvQueryFrame(cap);
       if(!src){ cout << "stream failed!" << endl; break;}

       cvWaitKey(5);
    }

}

typedef struct {
    int h;
    int w;
    int c;
    float *data;
} image;


void constrain_image(image im)
{
    for(int i = 0; i < im.w*im.h*im.c; ++i){
        if(im.data[i] < 0) im.data[i] = 0;
        if(im.data[i] > 1) im.data[i] = 1;
    }
}

image copy_image(image p)
{
    image copy = p;
    copy.data = (float*)calloc(p.h*p.w*p.c, sizeof(float));
    memcpy(copy.data, p.data, p.h*p.w*p.c*sizeof(float));
    return copy;
}

void free_image(image m)
{
    if(m.data){
        free(m.data);
    }
}

float get_pixel(image m, int x, int y, int c)
{
    assert(x < m.w && y < m.h && c < m.c);
    return m.data[c*m.h*m.w + y*m.w + x];
}



void rgbgr_image(image im)
{
    int i;
    for(i = 0; i < im.w*im.h; ++i){
        float swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}


image ipl_to_image(IplImage* src)
{
    unsigned char *data = (unsigned char *)src->imageData;
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    int step = src->widthStep;

    image out;
    out.data = 0;

    out.h = h;
    out.w = w;
    out.c = c;
    out.data = (float*)calloc(h*w*c, sizeof(float));

    int count=0;

    for(int k= 0; k < c; ++k){
        for(int i = 0; i < h; ++i){
            for(int j = 0; j < w; ++j){
                out.data[count++] = data[i*step + j*c + k]/255.;
            }
        }
    }
    return out;
}

image make_empty_image(int w, int h, int c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    return out;
}

//pad zeros
std::string ZeroPadNumber(int num)
{
    std::ostringstream ss;
    ss << std::setw( 7 ) << std::setfill( '0' ) << num;
    return ss.str();
}


// <<<<<<<<<*********************************
// <<<<<<<<< support functions **************
// <<<<<<<<<*********************************

// labeller C
extern "C" void label_func(int tl_x, int tl_y, int br_x, int br_y, char *names){

   string str(names);
   Scalar color;
   bool keep = false;
   bool animal = false;

   if(str == "person"){  //index 01
     color = Scalar(255, 0, 0);  //coral color
     keep = true;
   }else if (str == "dog"){ //index 03
     color = Scalar(0,255,0);      //gold color
     animal = true;
   }else if (str == "cat"){ //index 03
     color = Scalar(0,255,0);      //gold color
     animal = true;
   }else{
     color = Scalar(0,0,0);          //black
   }

   if(keep){
   //  cout << "What is this? Ans: " << str << endl;

     human_flag = true;
/*
     Point namePos(tl_x,tl_y-10);
     rectangle(img_cpp, Point(tl_x,tl_y), Point(br_x,br_y), color, 2, CV_AA);  //draw rectangle
     putText(img_cpp, str, namePos, FONT_HERSHEY_PLAIN, 2.0, color, 1.5);   //write the name of the object
*/
     int width = img_cpp.cols;
     int height = img_cpp.rows;

     if(tl_x < 0)
        tl_x = 0;
     if(tl_y < 0)
        tl_y = 0;


     if(br_x > width)
        br_x = width;

     if(br_y > height)
        br_y = height;

     cout << Point(tl_x,tl_y) << endl;
     cout << Point(br_x,br_y) << endl;

     Rect myROI(Point(tl_x,tl_y),Point(br_x,br_y));
     Mat croppedImage = img_cpp(myROI);
     //Mat finalized;

     //resize(croppedImage,finalized,Size(64,128));
     
     string path;
     path = "processed/human_pos/";
     path = path + "vo07_" + ZeroPadNumber(framecounter) + ".n00000001.jpg";

     if(imwrite(path, croppedImage))
        cout << "Recorded Human" << endl;

     framecounter++;
   }else{

     if(animal){

     int width = img_cpp.cols;
     int height = img_cpp.rows;

     if(tl_x < 0)
        tl_x = 0;
     if(tl_y < 0)
        tl_y = 0;


     if(br_x > width)
        br_x = width;

     if(br_y > height)
        br_y = height;

     cout << Point(tl_x,tl_y) << endl;
     cout << Point(br_x,br_y) << endl;

     Rect myROI(Point(tl_x,tl_y),Point(br_x,br_y));
     Mat croppedImage = img_cpp(myROI);
     //Mat finalized;

     //resize(croppedImage,finalized,Size(64,128));
     
     string path;
     path = "processed/animal_pos/";
     path = path + "voc07_" + ZeroPadNumber(framecounter) + ".n00000002.jpg";

     if(imwrite(path, croppedImage))
        cout << "Recorded Animal" << endl;

     framecounter++;

     }
   }

}

// input picture frame from file
extern "C" image load_image_cv(char *filename)
{
    IplImage* src = cvLoadImage(filename);

    string filepath(filename);
    img_cpp = imread(filepath, CV_LOAD_IMAGE_UNCHANGED);

    image out = ipl_to_image(src);
    cvReleaseImage(&src);
    rgbgr_image(out);
    return out;
}

extern "C" image load_stream_cv()
{
    src = cvQueryFrame(cap);
    if (!src){
       cout << "Warning: frame is empty! Check camera setup" << endl;
       return make_empty_image(0,0,0);
    }
    //only for ZED Stereo!
    //cvSetImageROI(src, cvRect(0, 0, src->width/2,src->height));
    //IplImage *dst = cvCreateImage (cvGetSize(src),src->depth, src->nChannels );
    //cvCopy(src, dst, NULL);
    //cvResetImageROI(src);
    //cvReleaseImage(dst);

    image im = ipl_to_image(src);
    rgbgr_image(im);
    return im;
}

// display frame
extern "C" void display_image_cv(image display){

    cout << "framing" << endl;
    image copy = copy_image(display);
    constrain_image(copy);
    if(display.c == 3) rgbgr_image(copy);

/*
    TODO: Debug
    Mat A(copy.w, copy.h, CV_8UC3, copy.data);

    free_image(copy);

    imshow("image",A);
*/

    IplImage *disp = cvCreateImage(cvSize(display.w,display.h), IPL_DEPTH_8U, display.c);
    int step = disp->widthStep;

    char buff[256];
    cvNamedWindow(buff, CV_WINDOW_NORMAL);

    for(int y = 0; y < display.h; ++y){
        for(int x = 0; x < display.w; ++x){
            for(int k= 0; k < display.c; ++k){
                disp->imageData[y*step + x*display.c + k] = (unsigned char)(get_pixel(copy,x,y,k)*255);
            }
        }
    }

    free_image(copy);
    cvShowImage(buff, disp);
    cvReleaseImage(&disp);
    cvWaitKey(0);
}


int main(){

  // pre set for positive images
  cv::String path1("raw/*.png"); //select only jpg
  vector<cv::String> fn1;
  vector<cv::Mat> frameStack;

  cv::String path2("raw/*.png"); //select only jpg
  vector<cv::String> fn2;


  // output file:
  cv::String outPos("traindata/pos/"); //pos
  cv::String outNeg("traindata/neg/"); //neg

  //set up proceedure
  setup_proceedure();

  cv::glob(path1,fn1,true); // recurse

  for (size_t k=0; k<fn1.size(); k++){

    std::string file = fn1[k].operator std::string();
    char *cstr = new char[file.length() + 1];
    strcpy(cstr, file.c_str());

    picture_detector(cstr);

    imshow("result",img_cpp);
/*
    if(!human_flag){

     //Rect myROI(Point(0,0),Point(64,128));
     //Mat croppedImage = img_cpp(myROI);
 
     string path;
     path = "processed/human_neg/";
     path = path + "inria_" + ZeroPadNumber(framecounter) + ".n00000003.jpg";

     if(imwrite(path, img_cpp))
        cout << "Recorded negative!" << endl;

     framecounter++;

    }else{
      human_flag = false;
    }

*/
    delete [] cstr;

    cvWaitKey(5);
  }



  return 0;
}
