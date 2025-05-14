#include <iostream>
#include <fstream>

#include "Vec3.h"
#include "color.h"

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    
    // Open file
    std::ofstream out("output.ppm");
    if (!out) {
        std::cerr << "Error: Could not open file for writing.\n";
        return 1;
    }

    // PPM header
    out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //render

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            // Normalized coordinates (0.0 to 1.0)
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);

            // Create a smooth color gradient using u and v
            double t = double(j) / (image_height - 1);  // t = 1 at top, 0 at bottom
            double r = 0.5 + 0.5 * (1.0 - t);           // lighter red as you go down
            double g = 0.7 + 0.3 * (1.0 - t);           // lighter green as you go down
            double b = 1.0;                             // full blue all the way


            // Convert [0,1] color values to [0,255]
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            // Output RGB pixel to file
            out << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }




    
    std::cerr << "\nDone\n";
    out.close(); //closes the file explicitly
    return 0;
}