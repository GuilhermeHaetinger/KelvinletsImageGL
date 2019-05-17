
#include "../include/glImageAbstraction.h"

vec2 initialPos;
vec2 nextPos;
bool button_down;
bool reset;
bool retard;
float brushSize;
bool gpu;
int shader;
int width;
int height;
GLuint idxb;
bool newVtx;

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

void updateVariableDescr()
{
  cout << "\33[2K\rRETARD: "<<  boolalpha << retard << " -- USING GPU: " << boolalpha << gpu << " -- NEW BRUSH SIZE: " << brushSize << "     ";
  fflush(stdout);
}

static void clickButtonHandler(GLFWwindow * window, int button, int action, int mods)
{
  if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
      if(action == GLFW_PRESS)
        {
          reset = false;
          button_down = true;
          double x, y;
          glfwGetCursorPos(window, &x, &y);
          initialPos = vec2(x, height - 1 - y);
          nextPos = vec2(x, height - 1 - y);
        }else if(action == GLFW_RELEASE)
        {
	  newVtx = true;
          reset = true;
          button_down = false;
        }
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT &&
             action == GLFW_PRESS
             )
    {
      retard = !retard;
      updateVariableDescr();
    }
}

static void cursorMovementHandler(GLFWwindow * window, double xpos, double ypos)
{
  if(button_down)
    {
      nextPos = vec2(xpos, height - 1 - ypos);
    }
}

static void scrollHandler(GLFWwindow * window, double xoffset, double yoffset)
{
  brushSize += 10 * yoffset;
  updateVariableDescr();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
          gpu = !gpu;
          updateVariableDescr();
    }
}

void setHandlers(GLFWwindow * window)
{
  glfwSetMouseButtonCallback(window, clickButtonHandler);
  glfwSetCursorPosCallback(window, cursorMovementHandler);
  glfwSetScrollCallback(window, scrollHandler);
  glfwSetKeyCallback(window, key_callback);
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
  updateVariableDescr();
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
                string pathToVertexShader, string pathToFragmentShader,
                float a, float b, float c
                )
{
  setVertexBuffer(rend);
  setIndexBuffer(rend);
  setColorBuffer(rend);

////
	GLfloat * positions = (GLfloat *) malloc(rend.getNumOfVertices() * sizeof(GLfloat));
	for(int i = 0; i < rend.getNumOfVertices(); i++)
	{
		positions[i] = i;
	}
	glGenBuffers(1, &idxb);	
  	glBindBuffer(GL_ARRAY_BUFFER, idxb);
  	glBufferData(GL_ARRAY_BUFFER,
       		rend.getNumOfVertices() * sizeof(GLfloat),
               	positions,
               	GL_STATIC_DRAW);

  	glEnableVertexAttribArray(2);
  	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
  	free(positions);

///
  string vertexShader = ParseShader(pathToVertexShader);
  string fragmentShader = ParseShader(pathToFragmentShader);

  shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);

  GLint a_location = glGetUniformLocation(shader, "a");
  GLint b_location = glGetUniformLocation(shader, "b");
  GLint c_location = glGetUniformLocation(shader, "c");
  GLint width_location = glGetUniformLocation(shader, "width");
  GLint height_location = glGetUniformLocation(shader, "height");
  
  glUniform1f(a_location, a);
  glUniform1f(b_location, b);
  glUniform1f(c_location, c);
  glUniform1i(width_location, width);
  glUniform1i(height_location, height);
  
  GLint x0_location = glGetUniformLocation(shader, "x0");
  GLint force_location = glGetUniformLocation(shader, "force");
  GLint retard_location = glGetUniformLocation(shader, "retard");
  GLint button_down_location = glGetUniformLocation(shader, "button_down");
  GLint reset_location = glGetUniformLocation(shader, "reset");
  GLint gpu_location = glGetUniformLocation(shader, "gpu");

  glUniform2fv(x0_location, 1, &initialPos[0]);
  vec2 dif = nextPos - initialPos;
  glUniform2fv(force_location, 1, &dif[0]);
  glUniform1i(retard_location, retard);
  glUniform1i(button_down_location, button_down);
  glUniform1i(reset_location, reset);
  glUniform1i(gpu_location, gpu);

//////
	
/*      
        GLuint tbo;
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, rend.getNumOfVertices() * 3 * sizeof(GLfloat), nullptr, GL_STATIC_READ);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

        glBeginTransformFeedback(GL_TRIANGLES);

        glDrawElements(GL_TRIANGLE_STRIP, rend.getNumOfIndices(), GL_UNSIGNED_INT, nullptr);

        glEndTransformFeedback();

        glFlush();

	GLfloat feed[rend.getNumOfVertices() *  3];	
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feed), feed);

	for(int i = 0; i < rend.getNumOfVertices() * 3; i+=3)
	{
		cout << feed[i] << "    " << feed[i+1] << "    " << feed[i+2] << endl;
	}
	*/
}

GLFWwindow * init(RenderableImage * rend,
                   string pathToVertexShader, string pathToFragmentShader,
                   const char * programName, float a, float b, float c
                  )
{
  initialPos = vec2(0, 512);
  nextPos = vec2(0, 512);
  button_down = false;
  retard = true;
  brushSize = 100.;
  gpu = true;
  newVtx = false;
  height = rend->getHeight();
  width = rend->getWidth();
  GLFWwindow * window;
  window = initWindow(*rend, programName);
  rend->initBuffers();
  initRender(*rend, pathToVertexShader, pathToFragmentShader, a, b, c);
  return window;
}

void setKelvinVariables(RenderableImage rend)
{
  GLint x0_location = glGetUniformLocation(shader, "x0");
  GLint force_location = glGetUniformLocation(shader, "force");
  GLint brushSize_location = glGetUniformLocation(shader, "brushSize");
  GLint retard_location = glGetUniformLocation(shader, "retard");
  GLint button_down_location = glGetUniformLocation(shader, "button_down");
  GLint reset_location = glGetUniformLocation(shader, "reset");
  GLint gpu_location = glGetUniformLocation(shader, "gpu");


  glUniform2fv(x0_location, 1, &initialPos[0]);
  vec2 dif = nextPos - initialPos;
  glUniform2fv(force_location, 1, &dif[0]);
  glUniform1f(brushSize_location, brushSize);
  glUniform1i(retard_location, retard);
  glUniform1i(button_down_location, button_down);
  glUniform1i(reset_location, reset);
  glUniform1i(gpu_location, gpu);
} 

void reRender(GLFWwindow * window, RenderableImage rend, GLfloat * vertices)
{
  glClear(GL_COLOR_BUFFER_BIT);

  setKelvinVariables(rend);

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

/*
	GLfloat * positions = (GLfloat *) malloc(rend.getNumOfVertices() * sizeof(GLfloat));
        for(int i = 0; i < rend.getNumOfVertices(); i++)
        {
                positions[i] = i;
 	}
	glBindBuffer(GL_ARRAY_BUFFER, idxb);
  	glBufferSubData(GL_ARRAY_BUFFER, 0,
                  rend.getNumOfVertices() * sizeof(GLfloat),
                  positions);

	glEnableVertexAttribArray(2);

	int sz = ((width - 1) * 2) * (height - 1) * 3 * 3;
//	cout << sz << "   " << rend.getNumOfIndices() << endl;
	
        GLuint tbo;
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, sz * sizeof(GLfloat), nullptr, GL_STATIC_READ);

        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

        glBeginTransformFeedback(GL_TRIANGLES);
*/
        glDrawElements(GL_TRIANGLE_STRIP, rend.getNumOfIndices(), GL_UNSIGNED_INT, nullptr);
/*
        glEndTransformFeedback();

        glFlush();

	GLfloat feed[sz];
//	cout << sizeof(sz) << endl;
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feed), feed);
	
	GLfloat* ve = (GLfloat*)malloc(rend.getNumOfVertices() * 2 * sizeof(GLfloat));
	rend.getVertices(ve);
	for(int i = 0; i < sz; i+=3)
	{
		ve[(int)feed[i+2]*2] = feed[i];
		ve[(int)feed[i+2]*2 + 1] = feed[i+1];
	}	

	
	if(newVtx)
	{
		newVtx = false;
		rend.setVertices(ve);	
	}
//

*/  
  glfwSwapBuffers(window);
  glfwPollEvents();
  //free(colors);
 // free(positions);
  //free(ve);
}

void feedbackVariables(vec2 * initPosition, vec2 * nextPosition, bool * btn_down, bool * ret, float * brSize, bool * gpuProcessing)
{
  *initPosition = initialPos;
  *nextPosition = nextPos;
  *btn_down =  button_down;
  *ret = retard;
  *brSize = brushSize;
  *gpuProcessing = gpu;
}
