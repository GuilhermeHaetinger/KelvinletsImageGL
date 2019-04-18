#ifndef RENDERABLE_IMAGE_HEADER
#define RENDERABLE_IMAGE_HEADER

#include "../include/Image.h"
#include <GLFW/glfw3.h>

class RenderableImage
{
 private:
  GLfloat * vertices;
  GLuint * indices;
  GLfloat * colors;

  void initVertices();
  void initIndices();
  void initColors(Image img);

 public:
  RenderableImage(Image img);

  GLfloat * getVertices();
  GLuint * getIndices();
  GLfloat * getColors();

};

#endif
