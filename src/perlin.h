#pragma once

#include "vec3.h"

#include <random>
#include <vector>

inline float trilinear_interp(vec3 c[2][2][2], float u, float v, float w)
{

	constexpr int dim = 2;
	float accum = 0.f;
	float uu = u * u * (3.f - 2.f * u);
	float vv = v * v * (3.f - 2.f * v);
	float ww = w * w * (3.f - 2.f * w);

	for(int i = 0; i < dim; i++)
		for(int j = 0; j < dim; j++)
			for(int k = 0; k < dim; k++)
			{
				vec3 weight_v(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1.f - uu)) * (j * vv + (1 - j) * (1.f - vv)) *
						 (k * ww + (1 - k) * (1.f - ww)) * dot(c[i][j][k], weight_v);
			}

	return accum;
}

class Perlin
{
public:
	float noise(const vec3& p) const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = static_cast<int>(floor(p.x()));
		int j = static_cast<int>(floor(p.y()));
		int k = static_cast<int>(floor(p.z()));

		constexpr int dim = 2;
		vec3 c[dim][dim][dim];

		for(int di = 0; di < dim; di++)
			for(int dj = 0; dj < dim; dj++)
				for(int dk = 0; dk < dim; dk++)
					c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
										   perm_z[(k + dk) & 255]];

		return trilinear_interp(c, u, v, w);
	}

	float turb(const vec3& p, int depth = 7) const
	{
		float accum = 0.f;
		vec3 temp_p = p;
		float weight = 1.f;
		for(int i = 0; i < depth; i++)
		{
			accum += weight * noise(temp_p);
			weight *= 0.5f;
			temp_p *= 2.f;
		}

		return fabs(accum);
	}

private:
	static std::vector<vec3> generate()
	{
		constexpr size_t vec_size = 256;
		std::vector<vec3> p;
		p.reserve(vec_size);

		std::mt19937 mt_engine(std::random_device{}());
		std::uniform_real_distribution<float> fdist(0.f, 0.9999f);

		for(size_t i = 0; i < vec_size; i++)
			p.emplace_back(-1.f + 2.f * fdist(mt_engine),
						   -1.f + 2.f * fdist(mt_engine),
						   -1.f + 2.f * fdist(mt_engine));

		return p;
	}

	static void permute(std::vector<int> p)
	{
		std::mt19937 mt_engine(std::random_device{}());
		std::uniform_real_distribution<float> fdist(0.f, 0.9999f);
		for(size_t i = p.size() - 1; i > 0; i--)
		{
			int target = static_cast<int>(fdist(mt_engine) * (static_cast<float>(i) + 1.f));
			auto tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
		return;
	}

	static std::vector<int> generate_perm()
	{
		constexpr size_t vec_size = 256;
		std::vector<int> p;
		p.reserve(vec_size);
		for(size_t i = 0; i < vec_size; i++) p.push_back(i);
		permute(p);

		return p;
	}

private:
	static std::vector<vec3> ranvec;
	static std::vector<int> perm_x, perm_y, perm_z;
};

std::vector<vec3> Perlin::ranvec = Perlin::generate();
std::vector<int> Perlin::perm_x = Perlin::generate_perm();
std::vector<int> Perlin::perm_y = Perlin::generate_perm();
std::vector<int> Perlin::perm_z = Perlin::generate_perm();
