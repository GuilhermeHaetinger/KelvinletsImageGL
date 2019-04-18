#include "CImg.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include "../include/ShaderCompiler.h"
#include "../include/Image.h"
#include "../include/KelvinletsObject.h"

using namespace std;
using namespace cimg_library;

KelvinletsObject * kelvin;
double initX, initY;
double nextPosX, nextPosY;
bool button_down;

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

CImg<unsigned char> loadImage(const char* filepath)
{
    CImg<unsigned char> image(filepath);
    if (access( filepath, F_OK ) == -1)
    {
        cout << "FILE NOT FOUND!" << endl;
        exit(2);
    }
    return image;
}

GLFWwindow* initContextAndLibraries(int width,
                                    int height,
                                    const char* programName)
{
    if (!glfwInit()) exit(3);
    GLFWwindow* window;
    window = glfwCreateWindow(width, height, programName, NULL, NULL);
    glfwSetWindowSizeLimits(window, width, height, width, height);

    if (!window)
    {
        glfwTerminate();
        exit(4);
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        cout << "ERROR LOADING GLEW!" << endl;
        exit(5);
    }
    return window;
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if(GLFW_PRESS == action)
      {
        button_down = true;
        glfwGetCursorPos(window, &initX, &initY);
      }
    else if(GLFW_RELEASE == action)
      {
        button_down = false;
        kelvin->setCheckpoint();
      }
  }
}

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
  if(button_down)
  {
    vec2 initPos = vec2(initX, initY);
    glfwGetCursorPos(window, &nextPosX, &nextPosY);
    vec2 dif = vec2(nextPosX, nextPosY) - initPos;
    kelvin->grab(initPos, dif, 200.0);
  }
}

int main(int argc, char* argv[])
{
    checkArgumentConsistency(argc);

    Image *img = new Image(argv[1]);
    kelvin = new KelvinletsObject(img, atof(argv[2]), atof(argv[3]));


    GLFWwindow* window;
    window = initContextAndLibraries(img->width, img->height, "Kelvinlets");
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    GLfloat * vertices = kelvin->getImage()->getVertices();
    GLuint * indices = kelvin->getImage()->getIndices();
    GLfloat * colors = kelvin->getImage()->getColors();

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 img->getNumOfVertices() * 2 * sizeof(GLfloat),
                 vertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);


    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 img->getNumOfIndices() * sizeof(GLuint),
                 indices, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 img->getNumOfVertices() * 3 * sizeof(GLuint),
                 colors, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


    string vs = ParseShader("./shaders/vertexShader.glsl");
    string fs = ParseShader("./shaders/fragmentShader.glsl");

    unsigned int shader = CreateShader(vs, fs);
    glUseProgram(shader);

    int i = 0;
    while (!glfwWindowShouldClose(window))
    {
        i++;
        glClear(GL_COLOR_BUFFER_BIT);
        vec2 initPos;
        vec2 nextPos;

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, img->getNumOfVertices() * 2 * sizeof(GLfloat), kelvin->getImage()->getVertices());
        glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, img->getNumOfVertices() * 3 * sizeof(GLfloat), kelvin->getImage()->getColors());
        glEnableVertexAttribArray(1);
        //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


        glDrawElements(GL_TRIANGLE_STRIP,
                       img->getNumOfIndices(),
                       GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();


    exit(0);
}
