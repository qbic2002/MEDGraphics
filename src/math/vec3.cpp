#include "vec3.h"

vec3::vec3() = default;

vec3::vec3(float xyz) : vec3(xyz, xyz, xyz) {}

vec3::vec3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec3 vec3::operator*(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

vec3 vec3::operator+(vec3 const &other) const {
    return {x + other.x, y + other.y, z + other.z};
}

vec3 vec3::operator+=(vec3 const &other) {
    return this->operator=(*this + other);
}

vec3 &vec3::operator=(vec3 const &other) = default;

