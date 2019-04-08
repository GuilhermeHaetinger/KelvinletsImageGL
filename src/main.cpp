#include "CImg.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

using namespace std;
using namespace cimg_library;

static string ParseShader(const string& filepath)
{
    ifstream stream(filepath);
    if (stream.fail())
    {
        cout << "NONEXISTANT FILE : \'" << filepath << "\'!" << endl;
    }

    stringstream ss;
    string line;
    while (getline(stream, line))
    {
        ss << line << endl;
    }
    return ss.str();
}

static unsigned int CompiledShader(unsigned int type,
                                   const string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        cout << "FAILED TO COMPILE SHADER : " << message << endl;
    }

    return id;
}

static unsigned int CreateShader(const string& vertexShader,
                                 const string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompiledShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompiledShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    CImg<unsigned char> image = loadImage(argv[1]);
    int width = image.width();
    int height = image.height();

    GLFWwindow* window;
    window = initContextAndLibraries(width, height, "Kelvinlets");

    GLfloat vertices[10] = {
        -0.5f,   0.0f,
        -0.25f,  0.0f,
         0.0f,   0.0f,
        -0.5f,  -0.5f,
        -0.25f, -0.5f
    };

    GLuint indices[5] = {
        0, 3, 1, 4, 2
    };

    GLfloat colors[20] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 10 * sizeof(GLfloat),
                 vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);


    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 5 * sizeof(GLuint),
                 indices, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(GLuint),
                 colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);


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

        glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
