#version 330 core

in vec4 toColor;
in vec2 toTexCoord;
out vec4 FragColor;

// texture sampler
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, toTexCoord);
}
