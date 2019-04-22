#ifndef IMAGE_HEADER
#define IMAGE_HEADER
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include "CImg.h"

using namespace cimg_library;
using namespace glm;

class Image
{
private:
    CImg<unsigned char> cimg_source;

public:
    Image(char* filepath);

    vec3 getColor(int x, int y);
    int getWidth();
    int getHeight();
};

#endif
