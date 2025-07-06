#include "utilities/base.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "objects/materials/dielectric.h"
#include "objects/materials/lambertian.h"
#include "objects/materials/metal.h"
#include "render/camera.h"


int main() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();

            if (point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); (center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;
    cam.set_aspect_ratio(16.0 / 9.0);
    cam.set_image_width(1200);
    cam.set_samples_per_pixel(500);
    cam.set_max_depth(50);
    cam.set_vertical_fov(20);
    cam.set_look_from({13,2,3});
    cam.set_look_at({0,0,0});
    cam.set_view_up({0,1,0});
    cam.set_focus_distance(10.0);
    cam.set_defocus_angle(0.6);
    cam.set_number_of_threads(32);

    cam.render(world);
}