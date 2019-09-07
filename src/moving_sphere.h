#pragma once

#include "hittable.h"
#include "material.h"

class MovingSphere : public Hittable
{
public:
	MovingSphere() = default;
	MovingSphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, std::shared_ptr<Material> m)
		: centre0(cen0)
		, centre1(cen1)
		, time0(t0)
		, time1(t1)
		, radius(r)
		, mat_ptr(m)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		vec3 oc = r.origin() - centre(r.time());
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
				rec.normal = (rec.p - centre(r.time())) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}

			temp = (-b + sqrt(b * b - a * c)) / a;
			if(temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - centre(r.time())) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
		}

		return false;
	}

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		AABB box_start =
			AABB(centre0 - vec3(radius, radius, radius), centre1 + vec3(radius, radius, radius));
		AABB box_end =
			AABB(centre1 - vec3(radius, radius, radius), centre1 + vec3(radius, radius, radius));
		box = AABB::surrounding_box(box_start, box_end);

		return true;
	}

	vec3 centre(float time) const
	{
		return centre0 + ((time - time0) / (time1 - time0)) * (centre1 - centre0);
	}

private:
	vec3 centre0, centre1;
	float time0, time1;
	float radius;
	std::shared_ptr<Material> mat_ptr;
};
