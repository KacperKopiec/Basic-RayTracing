#pragma once
#include "material.h"
#include "../../utilities/color.h"


class metal final : public material {
private:
    color albedo;
    double fuzz{};

public:
    metal() = default;

    explicit metal(const color& albedo, const double fuzz) : albedo(albedo), fuzz(fuzz < 1.0 ? fuzz : 1.0) {}

    bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override {
        vec3 reflected = vec3::reflect(r_in.get_direction(), rec.normal);
        reflected = unit_vector(reflected) + fuzz * vec3::random_unit_vector();
        scattered = ray{rec.p, reflected};
        attenuation = albedo;
        return (dot(scattered.get_direction(), rec.normal) > 0.0);
    }
};
