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

vec2 KelvinletsObject::grabVariation(vec2 position, vec2 force, vec2 actualPosition, float brushSize)
{
    // vec2 r = actualPosition - position;
    // float rLength = length(r);
    // float rEpslon = sqrt(pow(brushSize, 2), pow(rLength, 2));
    // cout << rEpslon << endl;
}
