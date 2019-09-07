#pragma once

#include "hittable.h"
#include "vec3.h"

#include <limits>
#include <memory>

class Util
{
public:
	static vec3 colour(const Ray& r, std::shared_ptr<Hittable> world, int depth)
	{
		HitRecord rec;
		if(world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
		{
			Ray scattered;
			vec3 attenuation;
			if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			{
				return attenuation * colour(scattered, world, depth + 1);
			}
			else
			{
				return vec3(0.f, 0.f, 0.f);
			}
		}
		else
		{
			vec3 unit_direction = unit_vector(r.direction());
			float t = 0.5f * (unit_direction.y() + 1.0f);
			return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.0f);
		}
	}
};
