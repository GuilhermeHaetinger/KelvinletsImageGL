#include "../include/KelvinletsObject.h"
#include <iostream>

using namespace glm;
using namespace std;

KelvinletsObject::KelvinletsObject(Image *source, float elasticShear, float poissonRatio)
{
    this->a = 1 / (4 * pi<float>() * poissonRatio);
    this->b = this->a / (4 - 4 * elasticShear);
    this->c = 2 / (3 * this->a - 2 * this->b);

    this->src = new Image(*source);
    this->newImage = new Image(*source);

    this->grabVariation(vec2(1, 0), vec2(1,0), vec2(2, 0), 50.);
}

void KelvinletsObject::reset(){this->newImage = this->src;}

void KelvinletsObject::grab(vec2 position, vec2 force, float brushSize)
{

}

void KelvinletsObject::grabRectangle(mat2 position, vec2 force, float brushSize)
{

}

void printVec(vec2 v){cout << "[" << v[0] << ", " << v[1] << "]"<< endl;}
void printMat(mat2 m){cout << "[" << m[0][0] << ", " << m[0][1] << endl << m[1][0] << ", " << m[1][1] << "]" << endl;}

mat2 productWithTranspost(vec2 x)
{
  return mat2(x[0] * x[0], x[1] * x[0], x[0] * x[1], x[1] * x[1]);
}

vec2 KelvinletsObject::grabVariation(vec2 position, vec2 force, vec2 actualPosition, float brushSize)
{
    vec2 r = actualPosition - position;
    float rLength = length(r);
    float rEpslon = sqrt(pow(brushSize, 2) + pow(rLength, 2));
    mat2 I = mat2();
    mat2 test = mat2(1, 1, 2, 2);
    printMat(test);
    mat2 kelvinState = ((this->a - this->b)/rEpslon) * I + (float)(this->b/pow(rEpslon, 3)) * productWithTranspost(r) + (float)((this->a / 2) * (pow(brushSize, 2)/pow(rEpslon, 3))) * I;
    printMat(kelvinState);
    printVec(this->c * brushSize * kelvinState * force);
}
