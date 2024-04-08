#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 toColor;
out vec2 toTexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
    toColor = aColor;
    toTexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
