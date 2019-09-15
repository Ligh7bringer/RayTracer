#pragma once

#include "aabb.h"
#include "hittable.h"

#include <limits>
#include <memory>

class RotateY : public Hittable
{
public:
    RotateY(std::shared_ptr<Hittable> p, float angle)
        : ptr(p)
    {
        float radians = (static_cast<float>(M_PI) / 180.f) * angle;
        sin_theta = sinf(radians);
        cos_theta = cosf(radians);
        has_box = ptr->bounding_box(0.f, 1.f, bbox);
        vec3 min(std::numeric_limits<float>::max(),
                 std::numeric_limits<float>::max(),
                 std::numeric_limits<float>::max());
        vec3 max(-std::numeric_limits<float>::max(),
                 -std::numeric_limits<float>::max(),
                 -std::numeric_limits<float>::max());
        constexpr int dim = 2;

        for(int i = 0; i < dim; i++)
        {
            for(int j = 0; j < dim; j++)
            {
                for(int k = 0; k < dim; k++)
                {
                    float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                    float y = i * bbox.max().y() + (1 - j) * bbox.min().y();
                    float z = i * bbox.max().z() + (1 - k) * bbox.min().z();
                    float newx = cos_theta * x + sin_theta * z;
                    float newz = -sin_theta * x + cos_theta * z;
                    vec3 tester(newx, y, newz);
                    for(int c = 0; c < 3; c++)
                    {
                        if(tester[c] > max[c]) max[c] = tester[c];

                        if(tester[c] < min[c]) min[c] = tester[c];
                    }
                }
            }
        }

        bbox = AABB(min, max);
    }

    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
    {
        vec3 origin = r.origin();
        vec3 direction = r.direction();
        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

        Ray rotated_r(origin, direction, r.time());
        if(ptr->hit(rotated_r, t_min, t_max, rec))
        {
            vec3 p = rec.p;
            vec3 normal = rec.normal;
            p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
            p[2] = -sin_theta * rec.normal[0] + cos_theta * rec.p[2];
            normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
            normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
            rec.p = p;
            rec.normal = normal;
            return true;
        }

        return false;
    }

    virtual bool bounding_box(float t0, float t1, AABB& box) const override { return false; }

private:
    std::shared_ptr<Hittable> ptr;
    float sin_theta, cos_theta;
    bool has_box;
    AABB bbox;
};
