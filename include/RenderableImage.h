#ifndef RENDERABLE_IMAGE_HEADER
#define RENDERABLE_IMAGE_HEADER

#include "../include/Image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace cv;

class RenderableImage
{
 private:
  GLfloat * vertices;
  GLuint * indices;
  GLfloat * colors;
  Image * src;

  GLuint vertexBuffer;
  GLuint indexBuffer;
  GLuint colorBuffer;

  int width;
  int height;

  void initVertices();
  void initIndices();
  void initColors(Image img);

  void refreshColors(Image img);

 public:
  RenderableImage(VideoCapture cap);

  void getVertices(GLfloat * dest);
  void getIndices(GLuint * dest);
  void getColors(GLfloat * dest);

  int getWidth();
  int getHeight();

  GLuint getVertexBuffer();
  GLuint getIndexBuffer();
  GLuint getColorBuffer();

  int getNumOfVertices();
  int getNumOfIndices();

  void initBuffers();

  void setVertices(GLfloat * newVertices);
};

#endif
