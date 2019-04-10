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

int main(int argc, char* argv[])
{
    checkArgumentConsistency(argc);

    Image *img = new Image(argv[1]);
    KelvinletsObject kelvin(img, atof(argv[2]), atof(argv[3]));

    GLfloat * pos;
    pos = img->getPosition(256, 256);

    GLFWwindow* window;
    window = initContextAndLibraries(img->width, img->height, "Kelvinlets");

    GLfloat * vertices = img->getVertices();
    GLuint * indices = img->getIndices();
    GLfloat * colors = img->getColors();

    // GLfloat vertices[18] = {
    //     -1.0f, 1.0f,
    //      0.0f, 1.0f,
    //      1.0f, 1.0f,
    //     -1.0f, 0.0f,
    //      0.0f, 0.0f,
    //      1.0f, 0.0f,
    //     -1.0f, -1.0f,
    //      0.0f, -1.0f,
    //      1.0f, -1.0f
    // };
    //
    // GLuint indices[16] = {
    //     0, 0, 3, 1, 4, 2, 5, 5, 3, 3, 6, 4, 7, 5, 8, 8
    // };
    //
    // GLfloat colors[36] = {
    //     1.0f, 0.0f, 0.0f,
    //     1.0f, 0.0f, 0.0f,
    //     1.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f,
    //     0.0f, 0.0f, 1.0f,
    //     0.0f, 0.0f, 1.0f,
    //     0.0f, 0.0f, 1.0f
    // };



    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 img->getNumOfVertices() * 2 * sizeof(GLfloat),
                 vertices, GL_STATIC_DRAW);
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
                 colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


    string vs = ParseShader("./shaders/vertexShader.glsl");
    string fs = ParseShader("./shaders/fragmentShader.glsl");

    unsigned int shader = CreateShader(vs, fs);
    glUseProgram(shader);

    // int location = glGetUniformLocation(shader, "u_Color");
    // assert(location != -1);
    // glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

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
