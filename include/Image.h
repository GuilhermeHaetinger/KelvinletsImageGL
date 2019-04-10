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

    int numOfVertices;
    int numOfIndices;

    GLfloat* vertices;
    GLuint* indices;
    GLfloat* colors;

    int calculateVtxInitPos(int x, int y);

    void initVertices();
    void initIndices();
    void initColors(CImg<unsigned char> image);

public:
    Image(char* filepath);
    Image(const Image& src);
    ~Image();

    int width;
    int height;

    GLfloat* getPosition(int x, int y);

    GLfloat* getVertices();
    GLuint* getIndices();
    GLfloat* getColors();

    int getNumOfVertices();
    int getNumOfIndices();

    void setPosition(int x, int y, GLfloat newX, GLfloat newY);

    void copy(Image * src);
};

#endif
