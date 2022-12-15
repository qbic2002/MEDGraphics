#version 330 compatibility

layout (location = 0) in vec3 position;

out vec3 in_position;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1.0f);
    in_position = position;
}