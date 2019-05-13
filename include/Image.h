#ifndef IMAGE_HEADER
#define IMAGE_HEADER
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include "CImg.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace glm;

class Image
{
private:
    Mat frame;
    VideoCapture src;

public:
    Image(VideoCapture cap);

    vec3 getColor(int x, int y);
    int getWidth();
    int getHeight();
    Mat getMat();
};

#endif
