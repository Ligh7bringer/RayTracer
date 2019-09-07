#pragma once

#include "vec3.h"

class Ray
{
public:
	Ray() = default;
	Ray(const vec3& a, const vec3& b, float ti = 0.f)
		: A(a)
		, B(b)
		, _time(ti)
	{}

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	float time() const { return _time; }

	vec3 point_at_parameter(float t) const { return A + t * B; }

private:
	vec3 A, B;
	float _time;
};
