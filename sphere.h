#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere: public hittable {
    public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {}

    virtual bool hit(
        const ray& r, double tmin, double tmax, hit_record& rec
    ) const;
    public:
    point3 center;
    double radius;
};

bool sphere::hit(
    const ray& r, double t_min, double t_max, hit_record& rec
) const {
    vec3 oc = o.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;   // 交点(最短距離)
        if (temp < t_max && temp>t_min) {   // 交点が基底の範囲内ならrecordする
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        } else {
            // 交点のもう一方もチェック
            temp = (-half_b + root) / a;
            if (temp < t_max && temp > t_min) {
                rec.t = temp;
                rec.p = r.at(rec.t);
                rec.normal = (rec.p - center) / radius;
                return true;
            }
        }
        return false;
    }
}

#endif