#pragma once
#include <thread>

#include "../objects/vec3.h"
#include "../objects/hittable.h"
#include "../objects/materials/material.h"


class camera {
private:
    double aspect_ratio{1.0};
    double pixel_samples_scale{0.01};
    double vertical_fov{90.0};
    double defocus_angle{0.0};
    double focus_distance{10.0};
    int image_width{100};
    int image_height{100};
    int samples_per_pixel{100};
    int max_depth{10};
    int number_of_threads{1};
    point3 center;
    point3 pixel00_loc;
    point3 look_from = point3{0,0,0};
    point3 look_at = point3{0,0,-1};
    vec3 view_up = vec3{0,1,0};
    vec3 u, v, w;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void init() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = look_from;

        const auto theta = degrees_to_radians(vertical_fov);
        const auto h = std::tan(theta / 2.0) * focus_distance;
        const auto viewport_height = 2.0 * h;
        const auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(view_up, w));
        v = cross(w, u);

        const auto viewport_u = viewport_width * u;
        const auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        const auto viewport_upper_left = center - focus_distance * w - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        const auto defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2.0));
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
    }

    static void process_scanlines(const camera& cam, const hittable& world, std::vector<std::vector<color>>& image, const int start, const int end) {
        for (int pixel = start; pixel < end; pixel++) {
            for (int sample = 0; sample < cam.samples_per_pixel; sample++) {
                const auto j = pixel / cam.image_width;
                const auto i = pixel % cam.image_width;
                const auto r = cam.get_ray(i, j);
                const auto pixel_color = cam.ray_color(r, cam.max_depth, world);
                image[j][i] += pixel_color;
            }
        }
    }

    [[nodiscard]] color ray_color(const ray& r, const int depth, const hittable& world) const {
        if (depth <= 0) return color{0, 0, 0};

        if (hit_record rec; world.hit(r, interval(0.001, infinity), rec)) {
            color attenuation;
            if (ray scattered; rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color{0, 0, 0};
        }

        const vec3 unit_direction = unit_vector(r.get_direction());
        const double a = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0};
    }

    [[nodiscard]] ray get_ray(const int i, const int j) const {
        const auto offset = sample_square();
        const auto pixel_sample = pixel00_loc + (i + offset.x) * pixel_delta_u + (j + offset.y) * pixel_delta_v;
        const auto ray_origin = defocus_angle <= 0 ? center : defocus_disk_sample();
        const auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    [[nodiscard]] point3 defocus_disk_sample() const {
        const auto p = vec3::random_unit_vector();
        return center + p.x * defocus_disk_u + p.y * defocus_disk_v;
    }

    static vec3 sample_square() {
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

public:
    void render(const hittable& world) {
        init();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        std::vector image(image_height, std::vector<color>(image_width));
        std::vector<std::thread> threads;

        for (int thread = 0; thread < number_of_threads; thread++) {
            const int start = static_cast<int>(static_cast<double>(thread) / number_of_threads * (image_height * image_width));
            const int end = static_cast<int>(static_cast<double>(thread + 1) / number_of_threads * (image_height * image_width));
            threads.emplace_back(process_scanlines, std::ref(*this), std::ref(world), std::ref(image), start, end);
        }

        for (auto &thread : threads) {
            thread.join();
        }

        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                write_color(std::cout, image[j][i] * pixel_samples_scale);
            }
        }
    }

    void set_aspect_ratio(const double aspect_ratio) {
        if (aspect_ratio <= 0.0) throw std::invalid_argument("Aspect ratio must be positive");
        this->aspect_ratio = aspect_ratio;
    }

    void set_image_width(const int image_width) {
        if (image_width <= 0.0) throw std::invalid_argument("Image width must be positive");
        this->image_width = image_width;
    }

    void set_samples_per_pixel(const int samples_per_pixel) {
        if (samples_per_pixel <= 0.0) throw std::invalid_argument("Samples per pixel must be positive");
        this->samples_per_pixel = samples_per_pixel;
    }

    void set_max_depth(const int max_depth) {
        if (max_depth <= 0.0) throw std::invalid_argument("Max depth must be positive");
        this->max_depth = max_depth;
    }

    void set_vertical_fov(const double vertical_fov) {
        if (vertical_fov <= 0.0) throw std::invalid_argument("Vertical field of view must be positive");
        this->vertical_fov = vertical_fov;
    }

    void set_look_from(const point3& look_from) {
        this->look_from = look_from;
    }

    void set_look_at(const point3& look_at) {
        this->look_at = look_at;
    }

    void set_view_up(const vec3& view_up) {
        this->view_up = view_up;
    }

    void set_focus_distance(const double focus_distance) {
        if (focus_distance <= 0.0) throw std::invalid_argument("Focus distance must be positive");
        this->focus_distance = focus_distance;
    }

    void set_defocus_angle(const double defocus_angle) {
        if (defocus_angle < 0.0) throw std::invalid_argument("Defocus angle must be non-negative");
        this->defocus_angle = defocus_angle;
    }

    void set_number_of_threads(const int number_of_threads) {
        if (number_of_threads <= 0) throw std::invalid_argument("Number of threads must be positive");
        this->number_of_threads = number_of_threads;
    }
};