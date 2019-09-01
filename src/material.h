#pragma once

#include <random>

#include "hittable.h"

vec3 random_in_unit_sphere()
{
	std::mt19937 mt_engine(std::random_device{}());
	std::uniform_real_distribution<float> fdist(0.f, 0.999f);
	vec3 p;

	do
	{
		p = 2.f * vec3(fdist(mt_engine), fdist(mt_engine), fdist(mt_engine)) - vec3(1.f, 1.f, 1.f);
	} while(p.squared_length() >= 1.f);

	return p;
}

class material
{
public:
	virtual bool
	scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const vec3& a)
		: albedo(a)
	{}

	virtual bool scatter(const ray& r_in,
						 const hit_record& rec,
						 vec3& attenuation,
						 ray& scattered) const override
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

private:
	vec3 albedo;
};

class metal : public material
{
public:
	metal(const vec3& a)
		: albedo(a)
	{}

	virtual bool scatter(const ray& r_in,
						 const hit_record& rec,
						 vec3& attenuation,
						 ray& scattered) const override
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	vec3 albedo;
};
