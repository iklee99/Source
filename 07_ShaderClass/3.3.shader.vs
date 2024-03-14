#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float val; 

out vec4 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = vec4(aColor[0] * val, aColor[1] * val, aColor[2] * val, 1.0);
}

