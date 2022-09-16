#version 330 compatibility

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec3 rotationAxis;
layout (location = 3) in float rotationAngle;
layout (location = 4) in float rotationVelocity;
layout (location = 5) in int index;

out vec3 ourColor;

uniform float theta;
uniform int width;
uniform int height;

float cycle(float value, float min, float max) {
    float range = max - min;
    //    if (value < min)  {
    //        value -= floor((value - min) / range) * range;
    //    }
    //    if (max < value) {
    //        value -= floor((value - min) / range) * range;
    //    }
    return value - floor((value - min) / range) * range;
}

void main()
{
    vec3 cornerVector = vec3[4](vec3(-1, -1, 0), vec3(1, -1, 0), vec3(1, 1, 0), vec3(-1, 1, 0))[index];

    vec3 pos = position + velocity * theta;
    pos.x = cycle(pos.x, -10, width + 10);
    pos.y = cycle(pos.y, -10, height + 10);

    float angle = rotationAngle + rotationVelocity * theta;
    vec4 q = vec4(rotationAxis * sin(angle / 2), cos(angle / 2));

    vec3 temp = cross(q.xyz, cornerVector) + q.w * cornerVector;
    cornerVector += 2.0 * cross(q.xyz, temp);

    pos += cornerVector * 5;

    ourColor = vec3(pos.z) / 100;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0f);
}