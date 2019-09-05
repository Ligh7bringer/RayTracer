#pragma once

#include "ray.h"

#include <memory>

class Material;

struct HitRecord
{
	float t;
	vec3 p;
	vec3 normal;
	std::shared_ptr<Material> mat_ptr;
};

class Hittable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};
