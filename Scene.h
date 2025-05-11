#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "HelperFunctions.h"
#include "Interval.h"
#include "Ray.h"
#include "Vector3.h"

class HitRecord {
public:
    Point3 hitPoint() const { return hitPoint_; }
    Vector3 surfaceNormal() const { return surfaceNormal_; }
    double distanceAlongRay() const { return distanceAlongRay_; }
    bool frontFace() const { return frontFace_; }

    // Setters for HitRecord members
    void setHitPoint(const Point3& hitPoint) { hitPoint_ = hitPoint; }
    void setSurfaceNormal(const Vector3& surfaceNormal) { surfaceNormal_ = surfaceNormal; }
    void setDistanceAlongRay(double distance) { distanceAlongRay_ = distance; }
    void setFrontFace(bool frontFace) { frontFace_ = frontFace; }

private:
    Point3 hitPoint_;
    Vector3 surfaceNormal_;
    double distanceAlongRay_ {infinity};
    bool frontFace_ {true};
};

class Object {
public:
    virtual std::optional<HitRecord> rayHit(const Ray& ray, Interval rayInterval) const = 0;
};

/*
 * TODO
class Sphere : public Object {
public:
    std::optional<HitRecord> rayHit(const Ray& ray, Interval rayInterval) const override {
        // Implement this function
    }
private:
    Point3 centre_ {0.0, 0.0, -1.0};
    double radius_ {0.5};
    // TODO
    // Material material_;
};
 */

// Sphere class implementation
class Sphere : public Object {
public:
    // Constructor for the Sphere class
    Sphere(const Point3& center, double radius) : center_(center), radius_(radius) {}

    // Override the rayHit function to check for intersections
    std::optional<HitRecord> rayHit(const Ray& ray, Interval rayInterval) const override {
        // Vector from ray origin to sphere center
        Vector3 oc = ray.origin() - center_;

        // Calculate the coefficients of the quadratic equation
        double a = ray.direction().dot(ray.direction());
        double b = 2.0 * oc.dot(ray.direction());
        double c = oc.dot(oc) - radius_ * radius_;

        // Calculate the discriminant
        double discriminant = b * b - 4.0 * a * c;

        // No intersection if discriminant is negative
        if (discriminant < 0) {
            return std::nullopt;
        }

        // Calculate the nearest intersection (t1 and t2 are the roots of the quadratic equation)
        double sqrtDiscriminant = sqrt(discriminant);
        double t1 = (-b - sqrtDiscriminant) / (2.0 * a);
        double t2 = (-b + sqrtDiscriminant) / (2.0 * a);

        // Ensure the intersection is within the ray's interval
        double t = (rayInterval.contains(t1)) ? t1 : t2;
        if (t < rayInterval.min() || t > rayInterval.max()) {
            return std::nullopt;  // Intersection outside the ray's valid interval
        }

        // Calculate the hit point and normal
        Point3 hitPoint = ray.pointAlongRay(t);
        Vector3 normal = (hitPoint - center_).unitVector();

        // Determine if the hit is on the front face of the sphere
        bool frontFace = ray.direction().dot(normal) < 0;

        // Return the hit record
        HitRecord hitRecord;
        hitRecord.setHitPoint(hitPoint);
        hitRecord.setSurfaceNormal(frontFace ? normal : -normal);
        hitRecord.setDistanceAlongRay(t);
        hitRecord.setFrontFace(frontFace);

        return hitRecord;
    }

private:
    Point3 center_;  // The center of the sphere
    double radius_;  // The radius of the sphere
};

// Plane class (commented out)
// TODO
// class Plane : public Object {
// public:
//     std::optional<HitRecord> rayHit(const Ray& ray, Interval rayInterval) const override {
//         // Implement this function
//     }
// private:
//     Point3 centre_ {0.0, 0.0, -1.0};
//     double radius_ {0.5};
//     // TODO
//     // Material material_;
// };

class Scene : public Object {
public:
    Scene(Object* o) {
        add(o);
    }

    void add(Object* o) {
        objects_.push_back(o);
    }

    void clear() {
        objects_.clear();
    }

    std::optional<HitRecord> rayHit(const Ray& ray, Interval rayInterval) const override {
        HitRecord tempHitRecord;
        bool hitAnything = false;
        double closestSoFar = rayInterval.max();

        for (const auto& o : objects_) {
            if (auto tempHit = o->rayHit(ray, Interval(rayInterval.min(), closestSoFar))) {
                hitAnything = true;
                closestSoFar = tempHit->distanceAlongRay();
                tempHitRecord = *tempHit;
            }
        }

        return hitAnything ? std::optional<HitRecord>{tempHitRecord} : std::nullopt;
    }

private:
    std::vector<Object*> objects_{};  // A collection of objects in the scene
};

#endif //RAYTRACER_SCENE_H
