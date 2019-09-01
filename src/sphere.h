#pragma once

#include "hittable.h"

class sphere : public hittable
{
public:
	sphere() = default;
	sphere(vec3 cen, float r, material* mat)
		: centre(cen)
		, radius(r)
		, mat_ptr(mat)
	{}

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		vec3 oc = r.origin() - centre;
		float a = dot(r.direction(), r.direction());
		float b = dot(oc, r.direction());
		float c = dot(oc, oc) - radius * radius;
		float discriminant = b * b - a * c;

		if(discriminant > 0.f)
		{
			float temp = (-b - sqrt(b * b - a * c)) / a;
			if(temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - centre) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}

			temp = (-b + sqrt(b * b - a * c)) / a;
			if(temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - centre) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
		}

		return false;
	}

private:
	vec3 centre;
	float radius;
	material* mat_ptr;
};
