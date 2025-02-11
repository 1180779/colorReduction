//
// Created on 11.02.2025.
//

#ifndef VEC4_HPP
#define VEC4_HPP

template<typename T = float>
struct vec4 {
    T x, y, z, w;

    vec4() : x(T()), y(T()), z(T()), w(T()) { }
    vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }

    vec4 operator+(const vec4 &other) const { return vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
    vec4 operator-(const vec4 &other) const { return vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
    vec4 operator*(const vec4 &other) const { return vec4(x * other.x, y * other.y, z * other.z, w * other.w); }
    vec4 operator/(const vec4 &other) const { return vec4(x / other.x, y / other.y, z / other.z, w / other.w); }

    vec4 operator+(const T other) const { return vec4(x + other, y + other, z + other, w + other); }
    vec4 operator-(const T other) const { return vec4(x - other, y - other, z - other, w - other); }
    vec4 operator*(const T other) const { return vec4(x * other, y * other, z * other, w * other); }
    vec4 operator/(const T other) const { return vec4(x / other, y / other, z / other, w / other); }
};

#endif //VEC4_HPP
