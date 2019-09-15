#pragma once

#include "aabb.h"
#include "hittable.h"

#include <memory>

class Translate : public Hittable
{
public:
    Translate(std::shared_ptr<Hittable> p, const vec3& displacement)
        : ptr(p)
        , offset(displacement)
    {}

    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
    {
        Ray moved_r(r.origin() - offset, r.direction(), r.time());
        if(ptr->hit(moved_r, t_min, t_max, rec))
        {
            rec.p += offset;
            return true;
        }
        else
            return false;
    }

    virtual bool bounding_box(float t0, float t1, AABB& box) const override
    {
        if(ptr->bounding_box(t0, t1, box))
        {
            box = AABB(box.max() + offset, box.max() + offset);
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    std::shared_ptr<Hittable> ptr;
    vec3 offset;
};
