#ifndef KELVINLETS_OBJECT_HEADER
#define KELVINLETS_OBJECT_HEADER
#include "../include/Image.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

class KelvinletsObject
{
private:
  Image* src;
  Image* newImage;
  float a;
  float b;
  float c;
  vec2 grabVariation(vec2 position, vec2 force, vec2 actualPosition, float brushSize);

public:
  KelvinletsObject(Image* source, float elasticShear, float poissonRatio);

  void reset();

  void grab(vec2 position, vec2 force, float brushSize);
  void grabRectangle(mat2 position, vec2 force, float brushSize);
};


#endif
