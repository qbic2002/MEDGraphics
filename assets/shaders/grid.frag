#version 330

out vec4 color;

uniform int size;
in vec3 in_position;

void main()
{
    color = vec4(vec3((mod(int(in_position.x/ size) + int(in_position.y/ size), 2) / 4 + 0.75)), 1);
}