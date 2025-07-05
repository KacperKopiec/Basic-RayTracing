#pragma once
#include "material.h"
#include "../../utilities/color.h"

class lambertian final : public material {
private:
    color albedo;

public:
    lambertian() = default;

    explicit lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + vec3::random_unit_vector();

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray{rec.p, scatter_direction};
        attenuation = albedo;
        return true;
    }
};
