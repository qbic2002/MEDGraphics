#version 330

out vec4 color;

uniform sampler2D texture0;

in vec4 fragColor;
in vec2 fragTexCoord;

void main()
{
    color = texture(texture0, fragTexCoord);
}