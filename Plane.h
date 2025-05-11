// In Plane.h
#include "Point3.h"
#include "Vector3.h"
#include "Ray.h"

class Plane : public Object {
public:
    Point3 point_;
    Vector3 normal_;

    Plane(const Point3& point, const Vector3& normal) : point_(point), normal_(normal) {}

    std::optional<HitRecord> rayHit(const Ray& ray, Interval rayInterval) const override {
        // Implement ray-plane intersection logic here
        // Return the hit record when an intersection occurs
    }
};
