#pragma once

#include <random>

#include "hittable.h"
#include "texture.h"

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

class Material
{
public:
	virtual bool
	scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
	virtual vec3 emitted(float u, float v, const vec3& p) const { return vec3(0.f, 0.f, 0.f); }
};

class Lambertian : public Material
{
public:
	Lambertian(std::shared_ptr<Texture> a)
		: albedo(a)
	{}

	virtual bool
	scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p, r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

private:
	std::shared_ptr<Texture> albedo;
};

class Metal : public Material
{
public:
	Metal(const vec3& a, float f)
		: albedo(a)
	{
		if(f < 1.f)
			fuzz = f;
		else
			fuzz = 1.f;
	}

	virtual bool
	scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	vec3 albedo;
	float fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(float ri)
		: ref_idx(ri)
	{}

	virtual bool
	scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override
	{
		vec3 outward_normal, refracted;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt, reflected_prob, cosine;
		attenuation = vec3(1.f, 1.f, 1.f);

		if(dot(r_in.direction(), rec.normal) > 0.f)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.f / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflected_prob = schlick(cosine, ref_idx);
		}
		else
		{
			//scattered = ray(rec.p, reflected);
			reflected_prob = 1.f;
		}

		std::mt19937 mt_engine(std::random_device{}());
		std::uniform_real_distribution<float> fdist(0.f, 0.999f);

		if(fdist(mt_engine) < reflected_prob)
		{
			scattered = Ray(rec.p, reflected);
		}
		else
		{
			scattered = Ray(rec.p, refracted);
		}

		return true;
	}

private:
	float ref_idx;
};

class DiffuseLight : public Material
{
public:
	DiffuseLight(std::shared_ptr<Texture> a)
		: emit(a)
	{}

	virtual bool
	scatter(const Ray& r_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const override
	{
		return false;
	}

	virtual vec3 emitted(float u, float v, const vec3& p) const override
	{
		return emit->value(u, v, p);
	}

private:
	std::shared_ptr<Texture> emit;
};
