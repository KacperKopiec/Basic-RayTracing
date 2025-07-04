#pragma once

#include "../objects/vec3.h"

using color = vec3;

inline void write_color(std::ostream& out, const color& pixel_color) {
    const int r = static_cast<int>(pixel_color.x * 255.999);
    const int g = static_cast<int>(pixel_color.y * 255.999);
    const int b = static_cast<int>(pixel_color.z * 255.999);

    out << r << ' ' << g << ' ' << b << '\n';
}