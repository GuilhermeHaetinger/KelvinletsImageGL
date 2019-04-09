#version 330 core

in vec3 fragmentColor;
layout(location = 0) out vec4 color;

void main()
{
   color = vec4(fragmentColor.xyz, 1.0f);
};
