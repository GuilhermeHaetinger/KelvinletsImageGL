#ifndef KELVINLETS_OBJECT_HEADER
#define KELVINLETS_OBJECT_HEADER
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat2x2.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>

using namespace glm;

class KelvinletsObject
{
 private:
  int width;
  int height;
  float a;
  float b;
  float c;

  GLfloat retardationFunction(float alpha);
  mat2 productWithTranspost(vec2 x);
  vec2 getPosition(int width, GLfloat * vertices, int x, int y);
  void retardInPosition(int x, int y, float * retardX, float * retardY);
  int calcVertexIndex(int width, int x, int y);
  void setVerticesPosition(int width, int x, int y, GLfloat * vertices, vec2 modif);
  vec2 grabVariation(vec2 position, vec2 force, vec2 actualPosition, float brushSize);

 public:
  KelvinletsObject(int width, int height, float elasticShear, float poissonRatio);

  void grab(vec2 position, vec2 force, float brushSize, GLfloat * src, GLfloat * dest);
  void grabRectangle(mat2 position, vec2 force, float brushSize);
};

#endif
