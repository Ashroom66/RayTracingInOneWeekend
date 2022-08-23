#include "rtweekend.h"

#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include "material.h"
#include "sphere.h"

#include <iostream>
using namespace std;


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;
    // 反射回数が一定値より多くなったら追跡中止
    if (depth<=0) {return color(0, 0, 0);}
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }
    // 背景のグラデーション
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main() {
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    cout    << "P3\n"
            << image_width << ' ' << image_height
            << "\n255\n";

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), -0.5, make_shared<dielectric>(1.51)));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.2, 0.8, 0.4))));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<metal>(color(0.8, 0.6, 0.2), 0.7)));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.45, make_shared<dielectric>(1.51)));

    camera cam;
    
    for (int j=image_height-1; j>=0; j--) {
        cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i=0; i<image_width; i++) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i+random_double()) / (image_width-1);
                auto v = (j+random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(cout, pixel_color, samples_per_pixel);
        }
    }
    cerr << "\nDone.\n";
}