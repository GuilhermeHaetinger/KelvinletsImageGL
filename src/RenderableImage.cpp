#include "../include/RenderableImage.h"
#include <iostream>
using namespace std;
using namespace cv;

void RenderableImage::initVertices()
{
  int size =  this->getNumOfVertices() * 2 * sizeof(GLfloat);
  GLfloat* v = (GLfloat*)malloc(size);
  int x, y;

  for(int i = 0; i < this->height; i++)
    for(int j = 0; j < this->width; j++)
    {
      int index = 2 * (j + i * this->width);
      x = index;
      y = index + 1;

      v[x] = 2*((GLfloat)j/(this->width - 1)) - 1;
      v[y] = 2*((GLfloat)((this->height - 1) - i)/(this->height - 1)) - 1;
    }
  this->vertices = v;
}

void RenderableImage::initIndices()
{
  int size =  this->getNumOfIndices() * sizeof(GLfloat);
  GLuint* ind = (GLuint*)malloc(size);

  int count = 0;

  for(int i = 0; i < this->height-1; i++)
  {
    ind[count] = i * this->width;
    count++;
    for(int j = 0; j < this->width; j++)
    {
      ind[count] = i*this->width + j;
      ind[count+1] = (i+1) * this->width + j;
      count+=2;
    }
    ind[count] = (i+1) * this->width + this->width - 1;
    count++;
  }
  this->indices = ind;
}

void RenderableImage::initColors(Image img)
{
  int size = this->getNumOfVertices() * 3 * sizeof(GLfloat);
  int bufS = this->getNumOfVertices() * 3 * sizeof(uint8_t);
  uint8_t * buff = (uint8_t*)malloc(bufS);
  GLfloat* c = (GLfloat*)malloc(size);

  Mat mat = img.getMat();
  memcpy(buff, mat.ptr(0), mat.cols*mat.rows * sizeof(uint8_t)*3);
  for(int i = 0; i < this->getNumOfVertices() * 3; i+=3)
  {
    c[i] = ((GLfloat)buff[i+2])/256;
    c[i+1] = ((GLfloat)buff[i+1])/256;
    c[i+2] = ((GLfloat)buff[i])/256;
  }
  free(buff);
  this->colors = c;
}

void RenderableImage::refreshColors(Image img)
{
  int size = this->getNumOfVertices() * 3 * sizeof(GLfloat);
  int bufS = this->getNumOfVertices() * 3 * sizeof(uint8_t);
  uint8_t * buff = (uint8_t*)malloc(bufS);	
  
  Mat mat = img.getMat();
  memcpy(buff, mat.ptr(0), mat.cols*mat.rows * sizeof(uint8_t)*3);
  for(int i = 0; i < this->getNumOfVertices() * 3; i+=3)
  {
    this->colors[i] = ((GLfloat)buff[i+2])/256;
    this->colors[i+1] = ((GLfloat)buff[i+1])/256;
    this->colors[i+2] = ((GLfloat)buff[i])/256;
  }
  free(buff);
}

RenderableImage::RenderableImage(VideoCapture cap)
{
  this->src = new Image(cap);
  this->width = (*this->src).getWidth();
  this->height = (*this->src).getHeight();
  this->initVertices();
  this->initIndices();
  this->initColors(*this->src);
}

void RenderableImage::getVertices(GLfloat * dest)
{memcpy(dest, this->vertices, this->getNumOfVertices() * 2 * sizeof(GLfloat));}

void RenderableImage::getIndices(GLuint * dest)
{memcpy(dest, this->indices, this->getNumOfIndices() * sizeof(GLuint));}

void RenderableImage::getColors(GLfloat * dest)
{
  this->refreshColors(*this->src);
  memcpy(dest, this->colors, this->getNumOfVertices() * 3 * sizeof(GLfloat));
}

GLuint RenderableImage::getVertexBuffer(){return this->vertexBuffer;}
GLuint RenderableImage::getIndexBuffer(){return this->indexBuffer;}
GLuint RenderableImage::getColorBuffer(){return this->colorBuffer;}

int RenderableImage::getWidth(){return this->width;}
int RenderableImage::getHeight(){return this->height;}

int RenderableImage::getNumOfVertices(){return this->width * this->height;}
int RenderableImage::getNumOfIndices()
{
  return (this->height - 1) * ((2 * this->width) + 2);
}

void RenderableImage::setVertices(GLfloat *newVertices)
{
  memcpy(this->vertices, newVertices, this->getNumOfVertices() * 2 * sizeof(GLfloat));
}

void RenderableImage::initBuffers()
{
  glGenBuffers(1, &(this->vertexBuffer));
  glGenBuffers(1, &(this->indexBuffer));
  glGenBuffers(1, &(this->colorBuffer)); 
}
