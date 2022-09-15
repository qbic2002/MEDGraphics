#version 330

out vec4 color;

uniform sampler2D texture0;

in vec2 TexCoord;

void main()
{
    color = texture(texture0, TexCoord);
}