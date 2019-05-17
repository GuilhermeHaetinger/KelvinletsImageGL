#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in float idx;

out vec3 fragmentColor;
out vec3 nPos;

void main()
{	
   nPos = vec3(1, 1, idx);
   fragmentColor = vertexColor;
   gl_Position = position;
};
