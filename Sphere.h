// In Sphere.h
#include "Point3.h"  // Assuming Point3 is defined properly
#include "Vector3.h" // Assuming Vector3 is defined properly
#include "Ray.h"

class Sphere : public Object {
public:
    Point3 center_;
    double radius_;

    Sphere(const Point3& center, double radius) : center_(center), radius_(radius) {}

    std::optional<HitRecord> rayHit(const Ray& ray, Interval rayInterval) const override {
        // Implement ray-sphere intersection logic here
        // Return the hit record when an intersection occurs
    }
};
