#pragma once

#include "aabb.h"
#include "hittable.h"
#include "material.h"

#include <memory>

class XYRect : public Hittable
{
public:
	XYRect() = default;
	XYRect(float _x0, float _x1, float _y0, float _y1, float _k, std::shared_ptr<Material> m)
		: x0(_x0)
		, x1(_x1)
		, y0(_y0)
		, y1(_y1)
		, k(_k)
		, mat_ptr(m)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		float t = (k - r.origin().z()) / r.direction().z();
		if(t < t_min || t > t_max) return false;

		float x = r.origin().x() + t * r.direction().x();
		float y = r.origin().y() + t * r.direction().y();
		if(x < x0 || x > x1 || y < y0 || y > y1) return false;

		rec.u = (x - x0) / (x1 - x0);
		rec.v = (y - y0) / (y1 - y0);
		rec.t = t;
		rec.mat_ptr = mat_ptr;
		rec.p = r.point_at_parameter(t);
		rec.normal = vec3(0.f, 0.f, 1.f);

		return true;
	}

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		box = AABB(vec3(x0, y0, k - 0.0001f), vec3(x1, y1, k + 0.0001f));
		return true;
	}

private:
	float x0, x1, y0, y1, k;
	std::shared_ptr<Material> mat_ptr;
};

class XZRect : public Hittable
{
public:
	XZRect() = default;
	XZRect(float _x0, float _x1, float _z0, float _z1, float _k, std::shared_ptr<Material> m)
		: x0(_x0)
		, x1(_x1)
		, z0(_z0)
		, z1(_z1)
		, k(_k)
		, mat_ptr(m)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		float t = (k - r.origin().y()) / r.direction().y();
		if(t < t_min || t > t_max) return false;

		float x = r.origin().x() + t * r.direction().x();
		float z = r.origin().z() + t * r.direction().z();
		if(x < x0 || x > x1 || z < z0 || z > z1) return false;

		rec.u = (x - x0) / (x1 - x0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.mat_ptr = mat_ptr;
		rec.p = r.point_at_parameter(t);
		rec.normal = vec3(0.f, 1.f, 0.f);

		return true;
	}

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		box = AABB(vec3(x0, k - 0.0001f, z0), vec3(x1, k + 0.0001f, z1));
		return true;
	}

private:
	float x0, x1, z0, z1, k;
	std::shared_ptr<Material> mat_ptr;
};

class YZRect : public Hittable
{
public:
	YZRect() = default;
	YZRect(float _y0, float _y1, float _z0, float _z1, float _k, std::shared_ptr<Material> m)
		: y0(_y0)
		, y1(_y1)
		, z0(_z0)
		, z1(_z1)
		, k(_k)
		, mat_ptr(m)
	{}

	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		float t = (k - r.origin().x()) / r.direction().x();
		if(t < t_min || t > t_max) return false;

		float y = r.origin().y() + t * r.direction().y();
		float z = r.origin().z() + t * r.direction().z();
		if(y < y0 || y > y1 || z < z0 || z > z1) return false;

		rec.u = (y - y0) / (y1 - y0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.mat_ptr = mat_ptr;
		rec.p = r.point_at_parameter(t);
		rec.normal = vec3(1.f, 0.f, 0.f);

		return true;
	}

	virtual bool bounding_box(float t0, float t1, AABB& box) const override
	{
		box = AABB(vec3(k - 0.0001f, y0, z0), vec3(k + 0.0001f, y1, z1));
		return true;
	}

private:
	float y0, y1, z0, z1, k;
	std::shared_ptr<Material> mat_ptr;
};
