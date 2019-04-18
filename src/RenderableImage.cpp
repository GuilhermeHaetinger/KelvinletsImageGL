#include "../include/RenderableImage.h"

void Image::initVertices()
{
  this->numOfVertices = this->width * this->height;
  int size =  this->numOfVertices * 2 * sizeof(GLfloat);
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

void Image::initIndices()
{
  this->numOfIndices = (this->height - 1) * ((2 * this->width) + 2);
  int size =  this->numOfIndices * sizeof(GLfloat);
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

void Image::initColors(CImg<unsigned char> image)
{
  int size = this->numOfVertices * 3 * sizeof(GLfloat);
  GLfloat* c = (GLfloat*)malloc(size);

  for(int i = 0; i < this->height; i++)
    for(int j = 0; j < this->width; j++)
      {
        int index = (j * 3) + (i * 3 * this->width);
        c[index] =     (GLfloat)image(j, i, 0, 0)/256; // R
        c[index + 1] = (GLfloat)image(j, i, 0, 1)/256; // G
        c[index + 2] = (GLfloat)image(j, i, 0, 2)/256; // B
      }
      this->colors = c;
}

