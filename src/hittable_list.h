#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

using hittables_vec = std::vector<std::shared_ptr<Hittable>>;

class HittableList : public Hittable
{
public:
	HittableList() = default;
	HittableList(const hittables_vec& l, int n)
		: list(l)
		, list_size(n)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
	{
		HitRecord temp_rec;
		bool hit_anything = false;
		float closest_so_far = t_max;
		for(int i = 0; i < list_size; i++)
		{
			if(list[i]->hit(r, t_min, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		if(list.size() < 1) return false;

		AABB temp_box;
		bool first_true = list[0]->bounding_box(t0, t1, temp_box);
		if(!first_true)
			return false;
		else
			box = temp_box;

		for(size_t i = 1; i < list.size(); i++)
		{
			if(list[0]->bounding_box(t0, t1, temp_box))
			{
				box = AABB::surrounding_box(box, temp_box);
			}
			else
				return false;
		}

		return true;
	}

private:
	hittables_vec list;
	int list_size;
};
