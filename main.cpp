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

// Plane class implementation
class Plane {
public:
    // Constructor for the Plane class
    Plane(Point3 point, Vector3 normal) : point_(point), normal_(normal.unitVector()) {}

    bool hit(const Ray& ray, double& t) const {
        double denominator = ray.direction().dot(normal_);

        // If the ray is parallel to the plane (denominator is close to 0)
        if (fabs(denominator) < 1e-6) {
            return false;  // No intersection
        }

        // Calculate the t value for the intersection point using the formula
        t = (point_ - ray.origin()).dot(normal_) / denominator;

        // If t is positive, the intersection is in front of the ray origin
        return t > 0;
    }

private:
    Point3 point_;  // A point on the plane
    Vector3 normal_;  // The normal vector of the plane
};

// Update rayColor to also check for plane intersections
Color3 rayColor(const Ray& r) {
    Point3 sphereCenter(0, 0, -1);
    double radius = 0.5;

    Vector3 oc = r.origin() - sphereCenter;
    double a = r.direction().length_squared();
    double half_b = oc.dot(r.direction());
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;

    // Check for sphere hit
    if (discriminant > 0) {
        double t = (-half_b - sqrt(discriminant)) / a;
        Vector3 normal = (r.pointAlongRay(t) - sphereCenter).unitVector();
        return Color3(1.0, 0.0, 0.0);  // Red color for sphere hit
    }

    // Define a plane (horizontal plane at y = -1 with normal pointing up)
    Plane plane(Point3(0.0, -1.0, 0.0), Vector3(0.0, 1.0, 0.0));  // Plane at y=-1, normal (0, 1, 0)

    double tPlane;
    // Check for plane hit
    if (plane.hit(r, tPlane)) {
        return Color3(0.0, 1.0, 0.0);  // Green color for plane hit
    }

    // Sky background color (gradient from white to blue)
    Vector3 unitDirection = r.direction().unitVector();
    double lerpFactor = 0.5 * (unitDirection.y() + 1.0);
    Color3 white(1.0, 1.0, 1.0);
    Color3 blue(0.5, 0.7, 1.0);
    return (1.0 - lerpFactor) * white + lerpFactor * blue;
}


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
