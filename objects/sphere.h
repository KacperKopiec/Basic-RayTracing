#pragma once
#include "hittable.h"

class sphere final : public hittable {
private:
    point3 center;
    double radius{0.0};

public:
    sphere() = default;

    explicit sphere(const point3& center, const double radius) : center(center), radius(std::fmax(0.0, radius)) {}

    [[nodiscard]] bool hit(const ray& r, const interval ray_t, hit_record& rec) const override {
        const vec3 oc = center - r.get_origin();
        const auto a = r.get_direction().squared_length();
        const auto h = dot(r.get_direction(), oc);
        const auto c = oc.squared_length() - radius * radius;

        const auto delta = h * h - a * c;
        if (delta < 0.0)
            return false;

        const auto sqrt_delta = std::sqrt(delta);
        auto root = (h - sqrt_delta) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrt_delta) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        const vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }
};
