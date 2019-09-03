#pragma once

#include "ray.h"

class material;

struct HitRecord
{
	float t;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
};

class Hittable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};
