#version 330

out vec4 color;

in vec3 ourColor;
in vec2 texCoord;
uniform sampler2D texture0;

void main()
{
    color = vec4(texture(texture0, texCoord).xyz * ourColor.r, 1);
    //    color = vec4(ourColor, 1);
}