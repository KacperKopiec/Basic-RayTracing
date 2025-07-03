#pragma once

#include <iostream>

class color {
private:
    constexpr static bool is_valid(const double value) noexcept {
        return 0.0 <= value && value <= 1.0;
    }

public:
    double r{0.0}, g{0.0}, b{0.0};

    constexpr color() noexcept = default;
    constexpr color(const double red, const double green, const double blue) {
        set(red, green, blue);
    }

    constexpr void set(const double red, const double green, const double blue) {
        if (!is_valid(red) || !is_valid(green) || !is_valid(blue)) {
            throw std::out_of_range("Color values must be in the range [0, 1]");
        }

        r = red;
        g = green;
        b = blue;
    }

    friend void write_color(std::ostream& out, const color& pixel_color) {
        const int r = static_cast<int>(pixel_color.r * 255.999);
        const int g = static_cast<int>(pixel_color.g * 255.999);
        const int b = static_cast<int>(pixel_color.b * 255.999);

        out << r << ' ' << g << ' ' << b << '\n';
    }

    friend constexpr color operator+(const color& u, const color& v) noexcept {
        return {u.r + v.r, u.g + v.g, u.b + v.b};
    }

    friend constexpr color operator*(const color& u, const double t) noexcept {
        return color{u.r * t, u.g * t, u.b * t};
    }

    friend constexpr color operator*(const double t, const color& u) noexcept {
        return color{u.r * t, u.g * t, u.b * t};
    }
};