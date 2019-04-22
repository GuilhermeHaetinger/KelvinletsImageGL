
#include "../include/glImageAbstraction.h"

vec2 initialPos;
vec2 nextPos;
bool button_down;
bool retard;
float brushSize;

void initGlfw()
{
  if (!glfwInit())
    {
      cout << "ERROR LOADING GLFW!" << endl;
      exit(3);
    }
}

void testWindow(GLFWwindow * window)
{
  if (!window)
    {
      cout << "WINDOW NOT SET PROPERLY!" << endl;
      glfwTerminate();
      exit(4);
    }
}

void initGlew()
{
  if (glewInit() != GLEW_OK)
    {
      cout << "ERROR LOADING GLEW!" << endl;
      exit(5);
    }
}

static void clickButtonHandler(GLFWwindow * window, int button, int action, int mods)
{
  if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
      if(action == GLFW_PRESS)
        {
          button_down = true;
          double x, y;
          glfwGetCursorPos(window, &x, &y);
          initialPos = vec2(x, y);
          nextPos = vec2(x, y);
        }else if(action == GLFW_RELEASE)
        {
          button_down = false;
        }
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT &&
             action == GLFW_PRESS
             )
      retard = !retard;
}

static void cursorMovementHandler(GLFWwindow * window, double xpos, double ypos)
{
  if(button_down)
    {
      nextPos = vec2(xpos, ypos);
    }
}

static void scrollHandler(GLFWwindow * window, double xoffset, double yoffset)
{
  brushSize += 10 * yoffset;
  fflush(stdout);
  cout << "\rNEW BRUSH SIZE: " << brushSize << "     ";
}

void setHandlers(GLFWwindow * window)
{
  glfwSetMouseButtonCallback(window, clickButtonHandler);
  glfwSetCursorPosCallback(window, cursorMovementHandler);
  glfwSetScrollCallback(window, scrollHandler);
}

GLFWwindow * initWindow(RenderableImage rend, const char * programName)
{
  initGlfw();
  int width = rend.getWidth();
  int height = rend.getHeight();
  GLFWwindow * window = glfwCreateWindow(width, height, programName, NULL, NULL);
  glfwSetWindowSizeLimits(window, width, height, width, height);
  testWindow(window);
  glfwMakeContextCurrent(window);
  initGlew();
  setHandlers(window);
  return window;
}

GLfloat * newVertexArray(RenderableImage rend)
{return (GLfloat *) malloc(rend.getNumOfVertices() * 2 * sizeof(GLfloat));}

void setVertexBuffer(RenderableImage rend)
{
  GLfloat * vertices = newVertexArray(rend);
  rend.getVertices(vertices);
  glBindBuffer(GL_ARRAY_BUFFER, rend.getVertexBuffer());
  glBufferData(GL_ARRAY_BUFFER,
               rend.getNumOfVertices() * 2 * sizeof(GLfloat),
               vertices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
  free(vertices);
}


GLuint * newIndexArray(RenderableImage rend)
{return (GLuint *) malloc(rend.getNumOfIndices() * sizeof(GLuint));}

void setIndexBuffer(RenderableImage rend)
{
  GLuint * indices = newIndexArray(rend);
  rend.getIndices(indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rend.getIndexBuffer());
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               rend.getNumOfIndices() * sizeof(GLuint),
               indices,
               GL_STATIC_DRAW);
  free(indices);
}

GLfloat * newColorArray(RenderableImage rend)
{return (GLfloat *) malloc(rend.getNumOfVertices() * 3 * sizeof(GLfloat));}

void setColorBuffer(RenderableImage rend)
{
  GLfloat * colors = newColorArray(rend);
  rend.getColors(colors);
  glBindBuffer(GL_ARRAY_BUFFER, rend.getColorBuffer());
  glBufferData(GL_ARRAY_BUFFER,
               rend.getNumOfVertices() * 3 * sizeof(GLfloat),
               colors,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  free(colors);
}

void initRender(RenderableImage rend,
                string pathToVertexShader, string pathToFragmentShader
                )
{
  setVertexBuffer(rend);
  setIndexBuffer(rend);
  setColorBuffer(rend);

  string vertexShader = ParseShader(pathToVertexShader);
  string fragmentShader = ParseShader(pathToFragmentShader);

  GLuint shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);
}

GLFWwindow * init(RenderableImage rend,
                   string pathToVertexShader, string pathToFragmentShader,
                   const char * programName
                  )
{
  initialPos = vec2(0, 0);
  nextPos = vec2(0, 0);
  button_down = false;
  retard = true;
  brushSize = 100.;
  GLFWwindow * window;
  window = initWindow(rend, programName);
  initRender(rend, pathToVertexShader, pathToFragmentShader);
  return window;
}

void reRender(GLFWwindow * window, RenderableImage rend, GLfloat * vertices)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBindBuffer(GL_ARRAY_BUFFER, rend.getVertexBuffer());
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  rend.getNumOfVertices() * 2 * sizeof(GLfloat), vertices);
  glEnableVertexAttribArray(0);

  GLfloat * colors = newColorArray(rend);
  rend.getColors(colors);

  glBindBuffer(GL_ARRAY_BUFFER, rend.getColorBuffer());
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  rend.getNumOfVertices() * 3 * sizeof(GLfloat),
                  colors);

  glEnableVertexAttribArray(1);
  glDrawElements(GL_TRIANGLE_STRIP,
                 rend.getNumOfIndices(),
                 GL_UNSIGNED_INT, nullptr);

  glfwSwapBuffers(window);
  glfwPollEvents();
  free(colors);
}

void feedbackVariables(vec2 * initPosition, vec2 * nextPosition, bool * btn_down, bool * ret, float * brSize)
{
  *initPosition = initialPos;
  *nextPosition = nextPos;
  *btn_down =  button_down;
  *ret = retard;
  *brSize = brushSize;
}
