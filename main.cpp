#include <iostream>
#include <fstream>
#include <cmath>

#include "Camera.h"
#include "Color3.h"
#include "Ray.h"


// Define a simple Sphere class
class Sphere {
public:
    Sphere(Point3 center, double radius) : center_(center), radius_(radius) {}

    bool hit(const Ray& ray, double& t) const {
        Vector3 oc = ray.origin() - center_;
        double a = ray.direction().dot(ray.direction());
        double b = 2.0 * oc.dot(ray.direction());
        double c = oc.dot(oc) - radius_ * radius_;
        double discriminant = b * b - 4.0 * a * c;

        if (discriminant > 0) {
            t = (-b - sqrt(discriminant)) / (2.0 * a);
            return t > 0;
        }
        return false;
    }

private:
    Point3 center_;
    double radius_;
};

void updateProgressBar(int rowsDone, int imageHeight) {
    float percentage = ((float)(imageHeight - rowsDone) / imageHeight) * 100;
    std::cout << "[";
    for (int i = 0; i < 50; i++) {
        if (i < (percentage / 2)) {
            std::cout << "#";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "] " << percentage << "% \r" << std::flush;
}

#include <cmath> // Add this at the top if not already

Color3 rayColor(const Ray& r) {
    Point3 sphereCenter(0, 0, -1);
    double radius = 0.5;

    Vector3 oc = r.origin() - sphereCenter;
    double a = r.direction().length_squared();
    double half_b = oc.dot(r.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        double t = (-half_b - sqrt(discriminant)) / a;
        Vector3 normal = (r.pointAlongRay(t) - sphereCenter).unitVector();
        return Color3(0.5 * (normal.x() + 1), 0.5 * (normal.y() + 1), 0.5 * (normal.z() + 1));
    }

    Vector3 unitDirection = r.direction().unitVector();
    double lerpFactor = 0.5 * (unitDirection.y() + 1.0);
    Color3 white(1.0, 1.0, 1.0);
    Color3 blue(0.5, 0.7, 1.0);
    return (1.0 - lerpFactor) * white + lerpFactor * blue;
}




int main() {
    int imageWidth = 512;
    int imageHeight = 512;
    std::string inFileName {};
    std::string outFileName {"image_7.ppm"};

    std::ofstream outFile(outFileName);

    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return 1;
    }

    Camera camera = Camera();

    // Define a sphere in the scene with adjusted radius and position
    Sphere sphere(Point3(0.0, 0.0, -1.0), 0.5);  // Sphere centered at (0, 0, -1) with radius 0.5

    outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
        for (int i = 0; i < imageWidth; i++) {
            auto ray = camera.getRay(i, j);

            auto pixelColor = rayColor(ray);


            outFile << pixelColor.r() << ' ' << pixelColor.g() << ' ' << pixelColor.b() << '\n';
        }
        updateProgressBar(j, imageHeight);
    }

    outFile.close();

    return 0;
}
