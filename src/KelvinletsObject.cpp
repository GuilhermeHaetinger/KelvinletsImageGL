#include "../include/KelvinletsObject.h"
#include <iostream>
using namespace glm;
using std::endl;
using std::cout;

KelvinletsObject::KelvinletsObject(int width, int height, float elasticShear, float poissonRatio)
{
    this->a = 1 / (4 * pi<float>() * poissonRatio);
    this->b = this->a / (4 - 4 * elasticShear);
    this->c = 2 / (3 * this->a - 2 * this->b);
    this->width = width;
    this->height = height;
}

void KelvinletsObject::grab(vec2 position, vec2 force, float brushSize, GLfloat * src, GLfloat * dest)
{
  vec2 delta;
  int index;

  for(int i = 0; i < this->height; i++)
    for(int j = 0; j < this->width; j++)
      {
        vec2 pos = getPosition(this->width, src, j, i);
        if(i == 200 && j == 200)
          cout << pos[0] << "   ===   " << pos[1] << endl;
        delta = grabVariation(position, force, pos, brushSize);

        delta += pos;
        delta[0] += 2 * (delta[0]/(this->width)) - 1;
        delta[1] += 2 * (delta[1]/(this->height)) - 1;


        // if(vec2(j, i) == position){
        //   cout << delta[0] << " ======== " << delta[1] << endl;
        // getchar();
        // }

        // GLfloat * retardX = (GLfloat*) malloc(sizeof(GLfloat));
        // GLfloat * retardY = (GLfloat*) malloc(sizeof(GLfloat));

        // retardInPosition(j, i, retardX, retardY);

        // delta[0] *= *retardX;
        // delta[1] *= *retardY;

        // vec2 pos = vec2(j, this->height - 1 - i);//getPosition(this->width, src, j, i);

        // pos[0] = 2 * (pos[0]/(this->width - 1));
        // pos[1] = 2 * (pos[1]/(this->height - 1));
        // if(vec2(j, i) == position){
        //   cout << delta[0] << " === " << delta[1] << endl << pos[0] << " ----- " << pos[1] << endl;
        //   getchar();
        // }
        // delta[0] = 2 * (delta[0]/(this->width - 1));
        // delta[1] = 2 * (delta[1]/(this->height - 1));
        // delta += pos;
        setVerticesPosition(this->width, j, i, dest, delta);
      }
}

void KelvinletsObject::grabRectangle(mat2 position, vec2 force, float brushSize){}

vec2 KelvinletsObject::grabVariation(vec2 position,
                                     vec2 force,
                                     vec2 actualPosition,
                                     float brushSize)
{
    vec2 r = actualPosition - position;
    float rLength = length(r);
    float rEpslon = sqrt(pow(brushSize, 2) + pow(rLength, 2));
    mat2 I = mat2(1.0f);
    mat2 first = (float) ((this->a - this->b)/rEpslon) * I;
    mat2 second = (float)(this->b/pow(rEpslon, 3)) * productWithTranspost(r);
    mat2 third = (float)((a/2) * ((pow(brushSize, 2)/pow(rEpslon, 3)))) * I;
    mat2 kelvinState = first + second + third;
    return this->c * brushSize * kelvinState * force;
}

int KelvinletsObject::calcVertexIndex(int width, int x, int y){return 2 * (x + y * width);}

vec2 KelvinletsObject::getPosition(int width, GLfloat * vertices, int x, int y)
{
  int index = calcVertexIndex(width, x, y);
  vec2 position = vec2(vertices[index], vertices[index + 1]);
  position[0] += (this->width) * (position[0] + 1)/2;
  position[1] += this->height - ((this->height) * (position[1] + 1)/2);
  return position;
}

void KelvinletsObject::setVerticesPosition(int width, int x, int y, GLfloat * vertices, vec2 modif)
{
  int index = calcVertexIndex(width, x, y);
  vertices[index] = modif[0];
  vertices[index + 1] = modif[1];
}

GLfloat KelvinletsObject::retardationFunction(float alpha)
{
  return (GLfloat)(1/(pow(2, 1/4))) * pow((float)cos(pi<float>() * alpha) + 1, (float)1/4);
}

void KelvinletsObject::retardInPosition(int x, int y, GLfloat * retardX, GLfloat * retardY)
{
  int dx1 = x;
  int dx2 = this->width - x - 1;
  int dy1 = y;
  int dy2 = this->height - y - 1;

  int dx = min(dx1, dx2);
  int dy = min(dy1, dy2);

  float propX = (float) (2 * (float)(this->width / 2 - dx) / (float)this->width);
  float propY = (float) (2 * (float)(this->height / 2 - dy) / (float)this->height);

  *retardX = retardationFunction(propX);
  *retardY = retardationFunction(propY);
}

mat2 KelvinletsObject::productWithTranspost(vec2 x)
{
  return mat2(x[0] * x[0], x[1] * x[0], x[0] * x[1], x[1] * x[1]);
}

