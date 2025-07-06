#pragma once
#include "material.h"
#include "../../utilities/color.h"


class dielectric final : public material {
private:
    double refraction_index{};

    [[nodiscard]] double reflectance(const double cosine) const {
        // Schlick's approximation
        auto r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
    }

public:
    dielectric() = default;

    explicit dielectric(const double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override {
        attenuation = color{1.0, 1.0, 1.0};
        const double refraction_ratio = rec.front_face ? (1.0 / refraction_index) : refraction_index;
        const vec3 unit_direction = unit_vector(r_in.get_direction());

        const double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        const double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        vec3 direction;
        if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta) > random_double())
            direction = vec3::reflect(unit_direction, rec.normal);
        else
            direction = vec3::refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray{rec.p, direction};
        return true;
    }
};
