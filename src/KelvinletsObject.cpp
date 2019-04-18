#include "../include/KelvinletsObject.h"
#include <iostream>
using namespace glm;
using std::endl;
using std::cout;

//AUXILIAR FUNCTIONS


GLfloat retardationFunction(float alpha)
{
  return (GLfloat)(1/(pow(2, 1/4))) * pow((float)cos(pi<float>() * alpha) + 1, (float)1/4);
}

void printVec(vec2 v){cout << "[" << v[0] << ", " << v[1] << "]"<< endl;}
void printMat(mat2 m){
  cout << "["
       << (float)m[0][0] << ", " << (float)m[0][1] << endl
       << (float)m[1][0] << ", " << (float)m[1][1]
       << "]" << endl;
}

mat2 productWithTranspost(vec2 x)
{
  return mat2(x[0] * x[0], x[1] * x[0], x[0] * x[1], x[1] * x[1]);
}

/////////////////

KelvinletsObject::KelvinletsObject(Image *source, float elasticShear, float poissonRatio)
{
    this->a = 1 / (4 * pi<float>() * poissonRatio);
    this->b = this->a / (4 - 4 * elasticShear);
    this->c = 2 / (3 * this->a - 2 * this->b);
    
    this->src = new Image(*source);
    this->newImage = new Image(*source);
    this->checkpoint = new Image(*source);
}

void KelvinletsObject::reset()
{
  free(this->newImage);
  this->newImage = new Image(*(this->src));
}

void KelvinletsObject::setVertices(GLfloat * vertices)
{
  this->newImage->setVertices(vertices);
}

void KelvinletsObject::resetFromCheckpoint()
{
  free(this->newImage);
  this->newImage = new Image(*(this->checkpoint));
}

void KelvinletsObject::setCheckpoint(){
  free(this->checkpoint);
  this->checkpoint = new Image(*(this->newImage));
}

void KelvinletsObject::grab(vec2 position, vec2 force, float brushSize, GLfloat * deposit)
{
  vec2 delta;
  int index;
  for(int i = 0; i < this->src->height; i++)
    for(int j = 0; j < this->src->width; j++)
      {
        delta = grabVariation(position, force, vec2(j, i), brushSize);
        int dx1 = j;
        int dx2 = this->src->width - j - 1;
        int dy1 = i;
        int dy2 = this->src->height - i - 1;

        int dx = min(dx1, dx2);
        int dy = min(dy1, dy2);

        float x = (float) (2 * (float)(this->src->width / 2 - dx) / (float)this->src->width);
        float y = (float) (2 * (float)(this->src->height / 2 - dy) / (float)this->src->height);

        delta[0] *= retardationFunction(x);
        delta[1] *= retardationFunction(y);

        GLfloat* buffpos = this->newImage->getPosition(j, i);
        buffpos[0] = ((buffpos[0] + 1)/2) * this->src->width;
        buffpos[1] = ((buffpos[1] + 1)/2) * this->src->height;

        delta += vec2(buffpos[0], buffpos[1]);
        delta[0] = 2 * (delta[0]/(this->src->width - 1)) - 1;
        delta[1] = 2 * (delta[1]/(this->src->height - 1)) - 1;

        index = 2 * (j + i * this->newImage->width);
        deposit[index] = delta[0];
        deposit[index + 1] = delta[1];
        //this->newImage->setPosition(j, i, delta[0], delta[1]);
      }
}

void KelvinletsObject::grabRectangle(mat2 position, vec2 force, float brushSize)
{

}

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

Image* KelvinletsObject::getImage(){return this->newImage;}
