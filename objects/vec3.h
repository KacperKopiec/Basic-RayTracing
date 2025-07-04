#pragma once

#include <stdexcept>

class vec3 {
public:
    double x{0.0}, y{0.0}, z{0.0};

    constexpr vec3() noexcept = default;
    constexpr vec3(const double x, const double y, const double z) noexcept : x(x), y(y), z(z) {}

    constexpr vec3 operator-() const noexcept { return {-x, -y, -z}; }

    constexpr vec3& operator+=(const vec3& other) noexcept {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    };

    constexpr vec3& operator-=(const vec3& other) noexcept {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr vec3& operator*=(const double t) noexcept {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    };

    constexpr vec3& operator/=(const double t) {
        if (t == 0.0) throw std::runtime_error("Division by zero");
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }

    [[nodiscard]] constexpr double squared_length() const noexcept {
        return x * x + y * y + z * z;
    }

    [[nodiscard]] constexpr double length() const noexcept {
        return std::sqrt(squared_length());
    }

    friend constexpr vec3 operator+(const vec3& u, const vec3& v) noexcept {
        return {u.x + v.x, u.y + v.y, u.z + v.z};
    }

    friend constexpr vec3 operator-(const vec3& u, const vec3& v) noexcept {
        return {u.x - v.x, u.y - v.y, u.z - v.z};
    }

    friend constexpr vec3 operator*(const vec3& u, const vec3& v) noexcept {
        return vec3{u.x * v.x, u.y * v.y, u.z * v.z};
    }

    friend constexpr vec3 operator*(const vec3& u, const double t) noexcept {
        return vec3{u.x * t, u.y * t, u.z * t};
    }

    friend constexpr vec3 operator*(const double t, const vec3& u) noexcept {
        return vec3{u.x * t, u.y * t, u.z * t};
    }

    friend constexpr vec3 operator/(const vec3& u, const double t) {
        if (t == 0.0) throw std::runtime_error("Division by zero");
        return vec3{u.x / t, u.y / t, u.z / t};
    }

    friend constexpr double dot(const vec3& u, const vec3& v) noexcept {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    friend constexpr vec3 cross(const vec3& u, const vec3& v) noexcept {
        return vec3{
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        };
    }

    friend vec3 unit_vector(const vec3& v) {
        const double len = v.length();
        if (len == 0.0) throw std::runtime_error("Cannot normalize a zero-length vector");
        return v / len;
    }

    friend std::ostream& operator<<(std::ostream& os, const vec3& v) {
        return os << v.x << ' ' << v.y << ' ' << v.z;
    }
};

using point3 = vec3;