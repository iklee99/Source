#version 330 core
uniform vec4 inColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(inColor);
}
