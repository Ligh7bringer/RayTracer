#pragma once

#include "vec3.h"

class Ray
{
public:
	Ray() = default;
	Ray(const vec3& a, const vec3& b)
		: A(a)
		, B(b)
	{}

	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(float t) const { return A + t * B; }

private:
	vec3 A, B;
};
