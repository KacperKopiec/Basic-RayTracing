#pragma once
#include "../objects/vec3.h"
#include "../objects/hittable.h"


class camera {
private:
    double aspect_ratio{1.0};
    int image_width{100};
    int image_height{100};
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void init() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3{0.0, 0.0, 0.0};

        constexpr auto focal_length = 1.0;
        constexpr auto viewport_height = 2.0;
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        const auto viewport_u = vec3{viewport_width, 0.0, 0.0};
        constexpr auto viewport_v = vec3{0.0, -viewport_height, 0.0};

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        const auto viewport_upper_left = center - vec3{0, 0, focal_length} - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    [[nodiscard]] color ray_color(const ray& r, const hittable& world) const {
        if (hit_record rec; world.hit(r, interval(0.0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1, 1, 1));
        }

        const vec3 unit_direction = unit_vector(r.get_direction());
        const double a = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0};
    }

public:
    void render(const hittable& world) {
        init();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << image_height - j << " " << std::flush;
            for (int i = 0; i < image_width; i++) {
                auto pixel_center = pixel00_loc + pixel_delta_u * i + pixel_delta_v * j;
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = ray_color(r, world);
                write_color(std::cout, pixel_color);
            }
        }

        std::clog << "\rDone.                  \n";
    }

    void set_aspect_ratio(const double aspect_ratio) {
        this->aspect_ratio = aspect_ratio;
    }

    void set_image_width(const int image_width) {
        this->image_width = image_width;
    }
};
