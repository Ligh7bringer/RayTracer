#pragma once

#include "hittable.h"
#include "util.h"

class Sphere : public Hittable
{
public:
	Sphere() = default;
	Sphere(vec3 cen, float r, std::shared_ptr<Material> mat)
		: centre(cen)
		, radius(r)
		, mat_ptr(mat)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
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
				Util::get_sphere_uv(rec.normal, rec.u, rec.v);
				return true;
			}

			temp = (-b + sqrt(b * b - a * c)) / a;
			if(temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - centre) / radius;
				rec.mat_ptr = mat_ptr;
				Util::get_sphere_uv(rec.normal, rec.u, rec.v);
				return true;
			}
		}

		return false;
	}

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		box = AABB(centre - vec3(radius, radius, radius), centre + vec3(radius, radius, radius));
		return true;
	}

private:
	vec3 centre;
	float radius;
	std::shared_ptr<Material> mat_ptr;
};
