#include "../include/glImageAbstraction.h"

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

static void clickButtonHandler(GLFWwindow* window, int button, int action, int mods)
{}

static void cursorMovementHandler(GLFWwindow* window, double xpos, double ypos)
{}

void setHandlers(GLFWwindow * window)
{
  glfwSetMouseButtonCallback(window, clickButtonHandler);
  glfwSetCursorPosCallback(window, cursorMovementHandler);
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

void setVertexBuffer(RenderableImage rend)
{
  glBindBuffer(GL_ARRAY_BUFFER, rend.getVertexBuffer());
  glBufferData(GL_ARRAY_BUFFER,
               rend.getNumOfVertices() * 2 * sizeof(GLfloat),
               rend.getVertices(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
}

void setIndexBuffer(RenderableImage rend)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rend.getIndexBuffer());
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               rend.getNumOfIndices() * sizeof(GLuint),
               rend.getIndices(),
               GL_STATIC_DRAW);
}

void setColorBuffer(RenderableImage rend)
{
  glBindBuffer(GL_ARRAY_BUFFER, rend.getColorBuffer());
  glBufferData(GL_ARRAY_BUFFER,
               rend.getNumOfVertices() * 3 * sizeof(GLfloat),
               rend.getColors(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0); 
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
  GLFWwindow * window;
  window = initWindow(rend, programName);
  initRender(rend, pathToVertexShader, pathToFragmentShader);
  return window;
}

void reRender(GLFWwindow * window, RenderableImage rend, GLfloat * vertices)
{
  glBindBuffer(GL_ARRAY_BUFFER, rend.getVertexBuffer());
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  rend.getNumOfVertices() * 2 * sizeof(GLfloat), vertices);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, rend.getColorBuffer());
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  rend.getNumOfVertices() * 3 * sizeof(GLfloat),
                  rend.getColors());
  
  glEnableVertexAttribArray(1);
}
