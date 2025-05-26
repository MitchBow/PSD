#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H

#include "material.h"
#include "Vec3.h"
#include "ray.h"
#include "hittable.h"

class checker_texture : public material {
public:
    checker_texture(const color& c1, const color& c2, double scale = 10.0)
        : odd(c1), even(c2), frequency(scale) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        auto sines = sin(frequency * rec.p.x())
                   * sin(frequency * rec.p.y())
                   * sin(frequency * rec.p.z());

        color checker_color = sines < 0 ? odd : even;
        scattered = ray(rec.p, random_unit_vector()); // diffuse scatter
        attenuation = checker_color;
        return true;
    }

private:
    color even;
    color odd;
    double frequency;
};

#endif
