#version 330 compatibility

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

out vec4 fragColor;
out vec2 fragTexCoord;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0f);
    fragColor = color;
    fragTexCoord = texCoord;
}