#version 330

out vec4 color;

in vec3 ourColor;

void main()
{
    color = vec4(ourColor, 1);
}