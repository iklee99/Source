#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec4 toColor;
uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
    toColor = aColor;
}
