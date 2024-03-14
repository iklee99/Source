#version 330 core
layout (location = 0) in vec3 aPos;
uniform float multY;

void main()
{
    gl_Position = vec4(aPos[0], aPos[1] * multY, aPos[2], 1.0);
}

