#version 330 compatibility

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec3 rotationAxis;
layout (location = 3) in float rotationAngle;
layout (location = 4) in float rotationVelocity;
layout (location = 5) in int index;

out float alpha;
out vec2 texCoord;

uniform float theta;
uniform int width;
uniform int height;

#define rectRadius 20

float cycle(float value, float min, float max) {
    float range = max - min;
    return value - floor((value - min) / range) * range;
}

void main()
{
    vec3 cornerVector = vec3[4](vec3(-1, -1, 0), vec3(1, -1, 0), vec3(1, 1, 0), vec3(-1, 1, 0))[index];
    texCoord = (cornerVector.xy + vec2(1, 1)) / 2;

    vec3 pos = position + velocity * theta;
    pos.x = cycle(pos.x, -rectRadius, width + rectRadius);
    pos.y = cycle(pos.y, -rectRadius, height + rectRadius);

    float angle = rotationAngle + rotationVelocity * theta;
    vec4 q = vec4(rotationAxis * sin(angle / 2), cos(angle / 2));

    vec3 temp = cross(q.xyz, cornerVector) + q.w * cornerVector;
    cornerVector += 2.0 * cross(q.xyz, temp);

    pos += cornerVector * rectRadius;

    alpha = pos.z / 100;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0f);
}