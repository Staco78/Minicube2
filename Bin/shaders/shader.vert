#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in float aTexIndex;
layout (location = 3) in float direction;

out vec2 TexCoords;
out flat float texIndex;
out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float NORTH = 0.0;
const float SOUTH = 1.0;
const float EAST = 2.0;
const float WEST = 3.0;
const float UP = 4.0;
const float DOWN = 5.0;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1);
    TexCoords = aTexCoords;
    texIndex = aTexIndex;

    vec3 light = vec3(1.0, 1.0, 1.0);
    if (direction == UP) {
        light *= 1.0;
    } else if (direction == EAST || direction == WEST) {
        light *= 0.8;
    } else if (direction == NORTH || direction == SOUTH) {
        light *= 0.6;
    } else {
        light *= 0.5;
    }

    color = vec4(light, 1.0);
}