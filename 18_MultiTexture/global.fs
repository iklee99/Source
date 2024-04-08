#version 330 core

in vec4 toColor;
in vec2 toTexCoord;
out vec4 FragColor;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2; 

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = mix(texture(texture1, toTexCoord), texture(texture2, toTexCoord), 0.2);
}
