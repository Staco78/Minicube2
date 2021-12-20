#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in flat float texIndex;

uniform sampler2DArray samplerArray;

void main()
{
    FragColor = texture(samplerArray, vec3(TexCoords, texIndex));
}