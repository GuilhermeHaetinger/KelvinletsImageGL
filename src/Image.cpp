#include "../include/Image.h"
#include <iostream>

using namespace std;
using namespace cimg_library;

Image::Image(char* filepath)
{
  CImg<unsigned char> image(filepath);
  this->cimg_source = image;
}

vec3 Image::getColor(int x, int y)
{
  GLfloat R =  (GLfloat)this->cimg_source(x, y, 0, 0)/256;
  GLfloat G =  (GLfloat)this->cimg_source(x, y, 0, 1)/256;
  GLfloat B =  (GLfloat)this->cimg_source(x, y, 0, 2)/256;

  return vec3(R, G, B);
}

int Image::getWidth(){return this->cimg_source.width();}
int Image::getHeight(){return this->cimg_source.height();}
