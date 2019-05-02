#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 vertexColor;

uniform float a;
uniform float b;
uniform float c;
uniform int width;
uniform int height;
uniform vec2 x0;
uniform vec2 force;
uniform float brushSize;
uniform int retard;
uniform int button_down;

out vec3 fragmentColor;

const float PI = 3.1415926535897932384626433832795;

vec2 normalizer(vec2 entry)
{
    vec2 outVec = vec2(entry[0], entry[1]);
    outVec[0] = 2 * (outVec[0]/(width)) - 1;
    outVec[1] = 2 * (outVec[1]/(height)) - 1;
    return outVec;
}

vec2 unormalize(vec2 entry)
{
    vec2 outVec = vec2(entry[0], entry[1]);
    outVec[0] = width * (outVec[0] + 1)/2;
    outVec[1] = height * (outVec[1] + 1)/2;
    return outVec;
}

float retardationFunction(float alpha)
{
  return float((1/(pow(2, 1/4))) * pow(float(cos(PI * alpha) + 1), float(1/4)));
}

vec2 retardInPosition(int x, int y)
{
    vec2 outRetard;

    int dx1 = x;
    int dx2 = width - x - 1;
    int dy1 = y;
    int dy2 = height - y - 1;

    int dx = min(dx1, dx2);
    int dy = min(dy1, dy2);

    float propX = float(2 * float(width / 2.0f - dx) / float(width));
    float propY = float(2 * float(height / 2.0f - dy) / float(height));

    outRetard[0] = retardationFunction(propX);
    outRetard[1] = retardationFunction(propY);
    return outRetard;
}

mat2 productWithTranspost(vec2 x)
{
  return mat2(x[0] * x[0], x[1] * x[0], x[0] * x[1], x[1] * x[1]);
}

vec2 grabVariation()
{
    vec2 unormPos = unormalize(position.xy);
    vec2 r = unormPos - x0;
    float rLength = length(r);
    float rEpslon = sqrt(pow(brushSize, 2) + pow(rLength, 2));
    mat2 I = mat2(1.0f);
    mat2 first = float((a - b)/rEpslon) * I;
    mat2 second = float(b/pow(rEpslon, 3)) * productWithTranspost(r);
    mat2 third = float((a/2) * ((pow(brushSize, 2)/pow(rEpslon, 3)))) * I;
    mat2 kelvinState = first + second + third;
    vec2 f = vec2(force[0], height - 1 - force[1]);
    return c * brushSize * kelvinState * f;
}

vec2 grab()
{
    vec2 unormPos = unormalize(position.xy);
    vec2 delta = grabVariation();
    if(bool(retard))
    {
        vec2 retard = retardInPosition(int(unormPos[0]), int(unormPos[1]));
        delta[0] *= retard[0];
        delta[1] *= retard[1];
    }
    delta += unormPos;
    return delta;
}

void main()
{   
    vec2 pos = grab();
    pos = normalizer(pos);
    fragmentColor = vertexColor;
    if(button_down == 1)
    {   
        gl_Position = vec4(pos[0], pos[1], 0.0f, 1.0f);
    }else{
        gl_Position = position;
    }
};