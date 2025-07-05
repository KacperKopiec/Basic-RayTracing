#pragma once

#include "interval.h"
#include "../objects/vec3.h"

using color = vec3;

inline double gamma_correction(const double linear_component) {
    return linear_component > 0.0 ? std::pow(linear_component, 1.0 / 2.2) : 0.0;
}

inline void write_color(std::ostream& out, const color& pixel_color) {
    static const interval intensity{0.000, 0.999};
    const int r = static_cast<int>(intensity.clamp(gamma_correction(pixel_color.x)) * 256);
    const int g = static_cast<int>(intensity.clamp(gamma_correction(pixel_color.y)) * 256);
    const int b = static_cast<int>(intensity.clamp(gamma_correction(pixel_color.z)) * 256);

    out << r << ' ' << g << ' ' << b << '\n';
}