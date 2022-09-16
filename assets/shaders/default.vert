#version 330 compatibility

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 ourColor;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0f);
    ourColor = color;
    TexCoord = vec2(texCoord.x, texCoord.y);
}