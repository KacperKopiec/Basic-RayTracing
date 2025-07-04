#pragma once

#include "interval.h"
#include "../objects/vec3.h"

using color = vec3;

inline void write_color(std::ostream& out, const color& pixel_color) {
    static const interval intensity{0.000, 0.999};
    const int r = static_cast<int>(intensity.clamp(pixel_color.x) * 256);
    const int g = static_cast<int>(intensity.clamp(pixel_color.y) * 256);
    const int b = static_cast<int>(intensity.clamp(pixel_color.z) * 256);

    out << r << ' ' << g << ' ' << b << '\n';
}