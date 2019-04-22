#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include "../include/KelvinletsObject.h"
#include "../include/glImageAbstraction.h"

using namespace std;

void checkArgumentConsistency(int argc)
{
    if (argc != 4)
    {
        cout << "INPUT FORMAT :"
        " ./kelvin [image filepath] [poisson ratio] [elastic shear modulus]"
        << endl;
        exit(1);
    }
}

void invertYAxis(vec2 * vec, int height)
{(*vec)[1] = height - 1 - (*vec)[1];}

int main(int argc, char* argv[])
{
    checkArgumentConsistency(argc);

    RenderableImage renderableImage(argv[1]);
    string vertex = "./shaders/vertexShader.glsl";
    string fragment = "./shaders/fragmentShader.glsl";
    GLFWwindow * window = init(renderableImage, vertex, fragment, "Kelvinlets");
    int width = renderableImage.getWidth();
    int height = renderableImage.getHeight();
    KelvinletsObject kelvin (width, height, atof(argv[2]), atof(argv[3]));

    vec2 * initialPosition = (vec2*) malloc(sizeof(vec2));
    vec2 * nextPosition = (vec2*) malloc(sizeof(vec2));
    bool * buttonDown  = (bool*) malloc(sizeof(bool));
    bool * retard = (bool*) malloc(sizeof(bool));
    float * brushSize  = (float*) malloc(sizeof(float));

    GLfloat * vertices = (GLfloat *)malloc(512 * 512 * 2 * sizeof(GLfloat));
    GLfloat * newVertices = (GLfloat *)malloc(512 * 512 * 2 * sizeof(GLfloat));
    renderableImage.getVertices(vertices);
    renderableImage.getVertices(newVertices);

    vec2 dif;
    while (!glfwWindowShouldClose(window))
      {
        feedbackVariables(initialPosition, nextPosition, buttonDown, retard, brushSize);
       if(*buttonDown)
        {
          invertYAxis(initialPosition, height);
          invertYAxis(nextPosition, height);
          dif = *nextPosition - *initialPosition;
          kelvin.grab(*initialPosition, dif, *brushSize, vertices, newVertices, *retard);
        }
      reRender(window, renderableImage, newVertices);
    }
    glfwTerminate();


    exit(0);
}
