#version 430

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_tex;

out vec2 tex_coords;

void main() {
    tex_coords = vertex_tex;
    gl_Position = vec4(vertex_position, 0, 1);
}