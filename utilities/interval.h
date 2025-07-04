#pragma once
#include "base.h"


class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {};

    interval(const double min, const double max) : min(min), max(max) {}

    [[nodiscard]] double size() const {
        return max - min;
    }

    [[nodiscard]] bool contains(const double x) const {
        return min <= x && x <= max;
    }

    [[nodiscard]] bool surrounds(const double x) const {
        return min < x && x < max;
    }

    [[nodiscard]] double clamp(const double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

const interval interval::empty{}, interval::universe{-infinity, infinity};
