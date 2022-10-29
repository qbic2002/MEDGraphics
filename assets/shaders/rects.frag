#version 330

out vec4 color;

in float alpha;
in vec2 texCoord;
uniform sampler2D texture0;

void main()
{
    color = vec4(texture(texture0, texCoord).xyz, alpha);
}