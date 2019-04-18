#ifndef IMAGE_HEADER
#define IMAGE_HEADER
#include <GLFW/glfw3.h>
#include <string>
#include "CImg.h"

using namespace cimg_library;

class Image
{
private:
    CImg<unsigned char> cimg_source;

public:
    Image(char* filepath);
    ~Image();

    vec3 getColor();
};

#endif
