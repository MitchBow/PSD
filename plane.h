#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"
#include "vec3.h"
#include "ray.h"

class plane : public hittable {
  public:
    point3 point;     // A point on the plane
    vec3 normal;      // The plane's normal vector
    shared_ptr<material> mat;

    plane() {}
    plane(const point3& p, const vec3& n, shared_ptr<material> m)
        : point(p), normal(unit_vector(n)), mat(m) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        double denom = dot(normal, r.direction());
        if (fabs(denom) < 1e-8)
            return false;  // Ray is parallel to the plane

        double t = dot(point - r.origin(), normal) / denom;
        if (!ray_t.surrounds(t))
            return false;

        rec.t = t;
        rec.p = r.at(t);
        rec.set_face_normal(r, normal);
        rec.mat = mat;
        return true;
    }

    bool bounding_box(double time0, double time1, aabb& output_box) const override {
    output_box = aabb(
        point3(-1e5, point.y() - 0.001, -1e5),
        point3( 1e5, point.y() + 0.001,  1e5)
    );
    return true;
}

};

#endif
