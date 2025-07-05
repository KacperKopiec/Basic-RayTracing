#pragma once
#include "ray.h"
#include "vec3.h"


class hit_record {
public:
    point3 p;
    vec3 normal;
    double t{0.0};
    bool front_face{false};

    constexpr hit_record() = default;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.get_direction(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
