#include "../include/RenderableImage.h"
#include <iostream>
using namespace std;

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
  GLfloat* c = (GLfloat*)malloc(size);

  for(int i = 0; i < this->height; i++)
    for(int j = 0; j < this->width; j++)
      {
        int index = (j * 3) + (i * 3 * this->width);
        vec3 pointColor = img.getColor(j, i);
        c[index] = pointColor[0];
        c[index + 1] = pointColor[1];
        c[index + 2] = pointColor[2];
      }
      this->colors = c;
}

RenderableImage::RenderableImage(char * filepath)
{
  Image img(filepath);
  this->width = img.getWidth();
  this->height = img.getHeight();
  this->initVertices();
  this->initIndices();
  this->initColors(img);
}

GLfloat * RenderableImage::getVertices(){return this->vertices;}
GLuint * RenderableImage::getIndices(){return this->indices;}
GLfloat * RenderableImage::getColors(){return this->colors;}

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
