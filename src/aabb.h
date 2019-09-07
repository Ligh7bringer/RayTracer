#pragma once

#include "ray.h"
#include "vec3.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB
{
public:
	AABB() = default;
	AABB(const vec3& a, const vec3& b)
		: _min(a)
		, _max(b)
	{}

	vec3 min() const { return _min; }
	vec3 max() const { return _max; }

	bool hit(const Ray& r, float tmin, float tmax) const
	{
		for(int a = 0; a < 3; a++)
		{
			//float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],
			//				 (_max[a] - r.origin()[a]) / r.direction()[a]);
			//float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],
			//				 (_max[a] - r.origin()[a]) / r.direction()[a]);
			//tmin = ffmax(t0, tmin);
			//tmax = ffmin(t1, tmax);

			float invD = 1.f / r.direction()[a];
			float t0 = (min()[a] - r.origin()[a]) * invD;
			float t1 = (max()[a] - r.origin()[a]) * invD;

			if(invD < 0.f) std::swap(t0, t1);

			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;

			if(tmax <= tmin) return false;
		}

		return true;
	}

private:
	vec3 _min, _max;
};

AABB surrounding_box(AABB box0, AABB box1)
{
	vec3 small(fmin(box0.min().x(), box1.min().x()),
			   fmin(box0.min().y(), box1.min().y()),
			   fmin(box0.min().z(), box1.min().z()));
	vec3 big(fmin(box0.max().x(), box1.max().x()),
			 fmin(box0.max().y(), box1.max().y()),
			 fmin(box0.max().z(), box1.max().z()));

	return AABB(small, big);
}
