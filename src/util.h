#pragma once

#include "hittable.h"
#include "vec3.h"

#include <limits>
#include <memory>

#define _USE_MATH_DEFINES

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
			vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
			if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
				return emitted + attenuation * colour(scattered, world, depth + 1);
			else
				return emitted;
		}
		else
			return vec3(0.f, 0.f, 0.f);
	}

	static void get_sphere_uv(const vec3& p, float& u, float& v)
	{
		float phi = atan2(p.z(), p.x());
		float theta = asin(p.y());
		u = 1 - (phi + M_PI) / (2 * M_PI);
		v = (theta + M_PI / 2) / M_PI;
	}
};
