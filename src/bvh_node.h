#pragma once

#include "aabb.h"
#include "hittable_list.h"

#include <algorithm>
#include <memory>
#include <random>

bool box_x_compare(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	AABB boxLeft, boxRight;
	if(!lhs->bounding_box(0.f, 0.f, boxLeft) || !rhs->bounding_box(0.f, 0.f, boxRight))
	{
		std::cerr << "No bounding box in BVHNode constructor\n";
	}

	return boxLeft.min().x() < boxRight.min().x();
}

bool box_y_compare(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	AABB boxLeft, boxRight;
	if(!lhs->bounding_box(0.f, 0.f, boxLeft) || !rhs->bounding_box(0.f, 0.f, boxRight))
	{
		std::cerr << "No bounding box in BVHNode constructor\n";
	}

	return boxLeft.min().y() < boxRight.min().y();
}

bool box_z_compare(const std::shared_ptr<Hittable>& lhs, const std::shared_ptr<Hittable>& rhs)
{
	AABB boxLeft, boxRight;
	if(!lhs->bounding_box(0.f, 0.f, boxLeft) || !rhs->bounding_box(0.f, 0.f, boxRight))
	{
		std::cerr << "No bounding box in BVHNode constructor\n";
	}

	return boxLeft.min().z() < boxRight.min().z();
}

class BVHNode : public Hittable
{
public:
	BVHNode() = default;
	BVHNode(hittables_vec list, float time0, float time1)
	{
		std::mt19937 mt_engine(std::random_device{}());
		std::uniform_real_distribution<float> fdist(0.f, 0.999f);
		int axis = static_cast<int>(3.f * fdist(mt_engine));

		if(axis == 0)
		{
			std::sort(list.begin(), list.end(), box_x_compare);
		}
		else if(axis == 1)
		{
			std::sort(list.begin(), list.end(), box_y_compare);
		}
		else
		{
			std::sort(list.begin(), list.end(), box_z_compare);
		}

		const auto n = static_cast<int>(list.size());
		if(list.size() == 1)
		{
			left = list[0];
			right = list[0];
		}
		else if(list.size() == 2)
		{
			left = list[0];
			right = list[1];
		}
		else
		{
			auto first = list.begin();
			auto last = list.end();

			std::vector<std::shared_ptr<Hittable>> leftHitables(first, first + n / 2);
			left = std::make_shared<BVHNode>(leftHitables, time0, time1);

			std::vector<std::shared_ptr<Hittable>> rightHitables(first + n / 2, last);
			right = std::make_shared<BVHNode>(rightHitables, time0, time1);
		}

		AABB box_left, box_right;
		if(!left->bounding_box(time0, time1, box_left) ||
		   !right->bounding_box(time0, time1, box_right))
		{
			std::cerr << "No bounding box in BVHNode ctor!\n";
			box = AABB::surrounding_box(box_left, box_right);
		}
	}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		if(box.hit(r, t_min, t_max))
		{
			HitRecord left_rec, right_rec;
			bool hit_left = left->hit(r, t_min, t_max, left_rec);
			bool hit_right = right->hit(r, t_min, t_max, right_rec);

			if(hit_left && hit_right)
			{
				if(left_rec.t < right_rec.t)
					rec = left_rec;
				else
					rec = right_rec;

				return true;
			}
			else if(hit_left)
			{
				rec = left_rec;
				return true;
			}
			else if(hit_right)
			{
				rec = right_rec;
				return true;
			}
			else
				return false;
		}

		return false;
	}

	virtual bool bounding_box(float t0, float t1, AABB& b) const override
	{
		b = box;
		return true;
	}

private:
	std::shared_ptr<Hittable> left, right;
	AABB box;
};
