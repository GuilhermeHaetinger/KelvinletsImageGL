#include "../include/Image.h"
#include <iostream>

using namespace std;
using namespace cv;

Image::Image(VideoCapture cap)
{
  this->src = cap;
  cap >> this->frame;
}

vec3 Image::getColor(int x, int y)
{
  Mat frame;
  this->src >> frame;
  Vec3b rgb = frame.at<Vec3b>(Point(x, y));
  GLfloat R =  ((float)rgb[0])/256;
  GLfloat G =  ((float)rgb[1])/256;
  GLfloat B =  ((float)rgb[2])/256;

  return vec3(R, G, B);
}

int Image::getWidth(){return this->src.get(CV_CAP_PROP_FRAME_WIDTH);}
int Image::getHeight(){return this->src.get(CV_CAP_PROP_FRAME_HEIGHT);}

Mat Image::getMat(){
  this->src >> this->frame;
  return this->frame;
}
