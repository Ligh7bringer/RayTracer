#pragma once

#include "hittable.h"

#include <memory>

class FlipNormals : public Hittable
{
public:
	FlipNormals(std::shared_ptr<Hittable> p)
		: ptr(p)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		if(ptr->hit(r, t_min, t_max, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		return ptr->bounding_box(t0, t1, box);
	}

private:
	std::shared_ptr<Hittable> ptr;
};
