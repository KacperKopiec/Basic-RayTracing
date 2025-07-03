#pragma once
#include "vec3.h"

class ray {
private:
    point3 origin;
    vec3 direction;

public:
    constexpr ray() = default;
    constexpr ray(const point3& origin, const vec3& direction) : origin(origin), direction(direction) {}

    [[nodiscard]] constexpr const point3& get_origin() const noexcept { return origin; }
    [[nodiscard]] constexpr const vec3& get_direction() const noexcept { return direction; }

    [[nodiscard]] constexpr point3 at(const double t) const noexcept { return origin + t * direction; }
};