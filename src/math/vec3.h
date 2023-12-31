#ifndef CPP_EXPERIMENTS_VEC3_H
#define CPP_EXPERIMENTS_VEC3_H

class vec3 {
public:
    float x{}, y{}, z{};

    vec3();

    explicit vec3(float xyz);

    vec3(float x, float y, float z);

    vec3 operator*(float scalar) const;

    vec3 operator+(const vec3& other) const;

    vec3 operator+=(const vec3& other);

    vec3& operator=(const vec3& other);
};

#endif //CPP_EXPERIMENTS_VEC3_H
