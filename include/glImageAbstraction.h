#ifndef OPENGL_IMAGE_ABSTRACTIONS
#define OPENGL_IMAGE_ABSTRACTIONS

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/RenderableImage.h"
#include "../include/ShaderCompiler.h"

GLFWwindow * init(RenderableImage rend,
                  string pathToVertexShader, string pathToFragmentShader,
                  const char * programName
                  );

void reRender(GLFWwindow * window, RenderableImage rend, GLfloat * vertices);

#endif
