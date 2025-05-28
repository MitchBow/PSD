//Mitchell Bowell 21610317
#include "rtweekend.h"
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "interval.h"
#include "camera.h"
#include "material.h"
#include "checker_texture.h"
#include "plane.h"

#include <fstream>
#include <iostream>


int main() {
    hittable_list world;

    bool antialiasing = true; //turn on or off antialiasing

    //floor
    auto ground_material = make_shared<checker_texture>(
        color(0.2, 0.8, 0.2),  // green
        color(1, 1, 1),  // white
        20.0            // higher frequency = smaller checks
    );

    world.add(make_shared<plane>(point3(0, -0.5, -1.5), vec3(0, 1, 0), ground_material));


    //sphere
    auto checker_sphere_mat = make_shared<checker_texture>(
        color(1, 0, 0),  // red
        color(1, 1, 1),  // white
        20.0            // higher frequency = smaller checks
    );

    world.add(make_shared<sphere>(point3(1.0, 0.1, -1.0), 0.5, checker_sphere_mat));


    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5)); //matt colour
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8)); //reflective metal
    
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;

    if(antialiasing == true){
        cam.samples_per_pixel = 100;
    }
    else{
        cam.samples_per_pixel = 30;
    }

    cam.max_depth = 50;

    cam.render(world);
}