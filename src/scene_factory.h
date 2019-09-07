#pragma once

#include "bvh_node.h"
#include "hittable.h"
#include "material.h"
#include "moving_sphere.h"
#include "sphere.h"
#include "texture.h"

#include <memory>
#include <vector>

class SceneFactory
{
public:
	static std::shared_ptr<Hittable> test_scene()
	{
		constexpr int list_size = 5;
		hittables_vec list(list_size);
		auto t0 = std::make_shared<ConstantTexture>(vec3(0.1f, 0.2f, 0.5f));
		auto t1 = std::make_shared<ConstantTexture>(vec3(0.8f, 0.8f, 0.f));

		list[0] =
			std::make_shared<Sphere>(vec3(0.f, 0.f, -1.f), 0.5f, std::make_shared<Lambertian>(t0));
		list[1] = std::make_shared<Sphere>(
			vec3(0.f, -100.5f, -1.f), 100.f, std::make_shared<Lambertian>(t1));
		list[2] = std::make_shared<Sphere>(
			vec3(1.f, 0.f, -1.f), 0.5f, std::make_shared<Metal>(vec3(0.8f, 0.6f, 0.2f), 0.3f));
		list[3] = std::make_shared<Sphere>(
			vec3(-1.f, 0.f, -1.f), 0.5f, std::make_shared<Dielectric>(1.5f));
		list[4] = std::make_shared<Sphere>(
			vec3(-1.f, 0.f, -1.f), -0.45f, std::make_shared<Dielectric>(1.5f));

		return std::make_shared<BVHNode>(list, 0.f, 1.f);
	}

	static std::shared_ptr<Hittable> random_scene()
	{
		constexpr int num_spheres = 11;
		hittables_vec hittables;
		hittables.reserve((2 * num_spheres) * (2 * num_spheres + 4));

		std::cout << "Generating a random scene... ";

		auto checker_tex = std::make_shared<CheckerTexture>(
			std::make_shared<ConstantTexture>(vec3(0.2f, 0.3f, 0.1f)),
			std::make_shared<ConstantTexture>(vec3(0.9f, 0.9f, 0.9f)));
		hittables.emplace_back(std::make_shared<Sphere>(
			vec3(0, -1000.f, 0.f), 1000.f, std::make_shared<Lambertian>(checker_tex)));

		std::mt19937 mt_engine(std::random_device{}());
		std::uniform_real_distribution<float> fdist(0.f, 0.999f);

		for(int a = -num_spheres; a < num_spheres; a++)
		{
			for(int b = -num_spheres; b < num_spheres; b++)
			{
				float choose_mat = fdist(mt_engine);
				vec3 centre(a + 0.9f * fdist(mt_engine), 0.2f, b + 0.9f * fdist(mt_engine));
				if((centre - vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
				{
					if(choose_mat < 0.8f) // diffuse
					{
						auto tex = std::make_shared<ConstantTexture>(
							vec3(fdist(mt_engine) * fdist(mt_engine),
								 fdist(mt_engine) * fdist(mt_engine),
								 fdist(mt_engine) * fdist(mt_engine)));
						hittables.emplace_back(std::make_shared<MovingSphere>(
							centre,
							centre + vec3(0.f, 0.5f * fdist(mt_engine), 0.f),
							0.f,
							1.f,
							0.2f,
							std::make_shared<Lambertian>(tex)));
					}
					else if(choose_mat < 0.95f) // metal
					{
						hittables.emplace_back(std::make_shared<Sphere>(
							centre,
							0.2f,
							std::make_shared<Metal>(vec3(0.5f * (1.f + fdist(mt_engine)),
														 0.5f * (1.f + fdist(mt_engine)),
														 0.5f * (1.f + fdist(mt_engine))),
													0.5f * fdist(mt_engine))));
					}
					else // glass
					{
						hittables.emplace_back(std::make_shared<Sphere>(
							centre, 0.2f, std::make_shared<Dielectric>(1.5f)));
					}
				}
			}
		}

		hittables.emplace_back(
			std::make_shared<Sphere>(vec3(0.f, 1.f, 0.f), 1.f, std::make_shared<Dielectric>(1.5f)));

		auto tex = std::make_shared<ConstantTexture>(vec3(0.4f, 0.2f, 0.1f));
		hittables.emplace_back(
			std::make_shared<Sphere>(vec3(-4.f, 1.f, 0.f), 1.f, std::make_shared<Lambertian>(tex)));

		hittables.emplace_back(std::make_shared<Sphere>(
			vec3(4.f, 1.f, 0.f), 1.f, std::make_shared<Metal>(vec3(0.7f, 0.6f, 0.5f), 0.f)));

		std::cout << "Done!\n";

		return std::make_shared<HittableList>(hittables, static_cast<int>(hittables.size()));
	}

	static std::shared_ptr<Hittable> two_spheres()
	{
		constexpr size_t num_spheres = 50;
		hittables_vec list;
		list.reserve(num_spheres + 1);

		auto checker_tex = std::make_shared<CheckerTexture>(
			std::make_shared<ConstantTexture>(vec3(0.2f, 0.3f, 0.1f)),
			std::make_shared<ConstantTexture>(vec3(0.9f, 0.9f, 0.9f)));

		list.emplace_back(std::make_shared<Sphere>(
			vec3(0.f, -10.f, 0.f), 10.f, std::make_shared<Lambertian>(checker_tex)));
		list.emplace_back(std::make_shared<Sphere>(
			vec3(0.f, 10.f, 0.f), 10.f, std::make_shared<Lambertian>(checker_tex)));

		return std::make_shared<HittableList>(list, 2);
	}
};
