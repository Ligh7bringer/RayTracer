#pragma once

#include "vec3.h"

#include <memory>

class Texture
{
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	ConstantTexture() = default;
	ConstantTexture(vec3 c)
		: colour(c)
	{}

	virtual vec3 value(float u, float v, const vec3& p) const override { return colour; }

private:
	vec3 colour;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture() = default;
	CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1)
		: even(t0)
		, odd(t1)
	{}

	virtual vec3 value(float u, float v, const vec3& p) const override
	{
		float sines = sin(10.f * p.x()) * sin(10.f * p.y()) * sin(10.f * p.z());
		if(sines < 0.f)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}

private:
	std::shared_ptr<Texture> odd, even;
};
