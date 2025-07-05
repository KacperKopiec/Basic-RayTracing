#pragma once
#include "../hit_record.h"
#include "../ray.h"
#include "../vec3.h"


class material {
public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        return false;
    }
};
