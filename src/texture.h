#pragma once

#include "perlin.h"
#include "vec3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    std::shared_ptr<Texture> even, odd;
};

class NoiseTexture : public Texture
{
public:
	NoiseTexture() = default;
    explicit NoiseTexture(float sc)
		: scale(sc)
	{}

	virtual vec3 value(float u, float v, const vec3& p) const override
	{
		//return vec3(1.f, 1.f, 1.f) * 0.5f * (1.f + noise.turb(scale * p));
		//return vec3(1.f, 1.f, 1.f) * noise.turb(scale * p);
		return vec3(1.f, 1.f, 1.f) * 0.5f * (1.f + sin(scale * p.z() + 10.f * noise.turb(p)));
	}

private:
	float scale;
	Perlin noise;
};

class ImageTexture : public Texture
{
public:
	ImageTexture() = default;
	ImageTexture(const std::string& filepath)
	{
		data = stbi_load(filepath.c_str(), &width, &height, &num_channels, 0);
	}
	~ImageTexture() { stbi_image_free(data); }

	virtual vec3 value(float u, float v, const vec3& p) const override
	{
        int i = static_cast<int>((u)*width);
        int j = static_cast<int>((1.f - v) * height - 0.001f);

		if(i < 0) i = 0;
		if(j < 0) j = 0;

		if(i > width - 1) i = width - 1;
		if(j > height - 1) j = height - 1;

        float r = static_cast<float>(int(data[3 * i + 3 * width * j]) / 255.0);
        float g = static_cast<float>(int(data[3 * i + 3 * width * j + 1]) / 255.0);
        float b = static_cast<float>(int(data[3 * i + 3 * width * j + 2]) / 255.0);

		return vec3(r, g, b);
	}

private:
	unsigned char* data;
	int width, height, num_channels;
};
