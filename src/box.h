#pragma once

#include "aabb.h"
#include "flip_normals.h"
#include "hittable_list.h"
#include "material.h"
#include "rect.h"
#include "vec3.h"

#include <memory>
#include <vector>

class Box : public Hittable
{
public:
    Box() = default;
    Box(const vec3& p0, const vec3& p1, std::shared_ptr<Material> mat)
        : pmin(p0)
        , pmax(p1)
    {
        constexpr int list_size = 6;
        hittables_vec list;
        list.reserve(list_size);

        list.emplace_back(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
        list.emplace_back(std::make_shared<FlipNormals>(
            std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat)));
        list.emplace_back(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
        list.emplace_back(std::make_shared<FlipNormals>(
            std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.x(), mat)));
        list.emplace_back(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
        list.emplace_back(std::make_shared<FlipNormals>(
            std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat)));

        list_ptr = std::make_shared<HittableList>(list, list_size);
    }

    virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
    {
        return list_ptr->hit(r, t_min, t_max, rec);
    }

    virtual bool bounding_box(float t0, float t1, AABB& box) const override
    {
        box = AABB(pmin, pmax);
        return true;
    }

private:
    vec3 pmin, pmax;
    std::shared_ptr<HittableList> list_ptr;
};
