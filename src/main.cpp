#include "CImg.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace cimg_library;

struct ShaderProgramSource
{
    string VertexSource;
    string FragmentSource;
};

static ShaderProgramSource ParseShader(const string& filepath)
{
        ifstream stream(filepath);
        if (stream.fail())
        {
            cout << "NONEXISTANT FILE!" << endl;
        }

        enum class ShaderType{
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        stringstream ss[2];
        string line;
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != string::npos)
            {
                if (line.find("vertex") != string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != string::npos)
                    type = ShaderType::FRAGMENT;
            } else {
                ss[(int)type] << line << endl;
            }
        }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompiledShader(unsigned int type, const string& source)
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

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
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

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
        cout << "ERROR LOADING GLEW!" << endl;

    GLfloat vertices[10] = {
        -0.5f,  -0.5f,
        -0.5f,   0.0f,
        -0.25f, -0.5f,
        -0.25f,  0.0f,
         0.0f,   0.0f
    };

    GLuint indices[9] = {
        0, 1, 3,
        0, 2, 3,
        2, 3, 4
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
    glBufferData(GL_ARRAY_BUFFER, 10 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);


    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    // GLuint colorBuffer;
    // glGenBuffers(1, &colorBuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    // glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    ShaderProgramSource source = ParseShader("./shaders/test.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
