#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "timer.h"

vec3 colour(const Ray& r, std::shared_ptr<Hittable> world, int depth)
{
	HitRecord rec;
	if(world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
	{
		Ray scattered;
		vec3 attenuation;

		if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * colour(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0.f, 0.f, 0.f);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

std::shared_ptr<Hittable> test_scene()
{
	constexpr int list_size = 5;
	hittables_vec list(list_size);

	list[0] = std::make_shared<Sphere>(
		vec3(0.f, 0.f, -1.f), 0.5f, std::make_shared<Lambertian>(vec3(0.1f, 0.2f, 0.5f)));
	list[1] = std::make_shared<Sphere>(
		vec3(0.f, -100.5f, -1.f), 100.f, std::make_shared<Lambertian>(vec3(0.8f, 0.8f, 0.f)));
	list[2] = std::make_shared<Sphere>(
		vec3(1.f, 0.f, -1.f), 0.5f, std::make_shared<Metal>(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] =
		std::make_shared<Sphere>(vec3(-1.f, 0.f, -1.f), 0.5f, std::make_shared<Dielectric>(1.5f));
	list[4] =
		std::make_shared<Sphere>(vec3(-1.f, 0.f, -1.f), -0.45f, std::make_shared<Dielectric>(1.5f));

	return std::make_shared<HittableList>(list, list_size);
}

std::shared_ptr<Hittable> random_scene()
{
	constexpr int num_spheres = 500;
	hittables_vec hittables(num_spheres + 1);

	std::cout << "Generating a random scene... ";
	hittables[0] = std::make_shared<Sphere>(
		vec3(0, -1000.f, 0.f), 1000.f, std::make_shared<Lambertian>(vec3(0.5f, 0.5f, 0.5f)));
	int idx = 1;

	std::mt19937 mt_engine(std::random_device{}());
	std::uniform_real_distribution<float> fdist(0.f, 0.999f);

	for(int a = -11; a < 11; a++)
	{
		for(int b = -11; b < 11; b++)
		{
			float choose_mat = fdist(mt_engine);
			vec3 centre(a + 0.9f * fdist(mt_engine), 0.2f, b + 0.9f * fdist(mt_engine));
			if((centre - vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
			{
				if(choose_mat < 0.8f) // diffuse
				{
					hittables[idx++] = std::make_shared<Sphere>(
						centre,
						0.2f,
						std::make_shared<Lambertian>(vec3(fdist(mt_engine) * fdist(mt_engine),
														  fdist(mt_engine) * fdist(mt_engine),
														  fdist(mt_engine) * fdist(mt_engine))));
				}
				else if(choose_mat < 0.95f) // metal
				{
					hittables[idx++] = std::make_shared<Sphere>(
						centre,
						0.2f,
						std::make_shared<Metal>(vec3(0.5f * (1.f + fdist(mt_engine)),
													 0.5f * (1.f + fdist(mt_engine)),
													 0.5f * (1.f + fdist(mt_engine))),
												0.5f * fdist(mt_engine)));
				}
				else // glass
				{
					hittables[idx++] =
						std::make_shared<Sphere>(centre, 0.2f, std::make_shared<Dielectric>(1.5f));
				}
			}
		}
	}

	hittables[idx++] =
		std::make_shared<Sphere>(vec3(0.f, 1.f, 0.f), 1.f, std::make_shared<Dielectric>(1.5f));
	hittables[idx++] = std::make_shared<Sphere>(
		vec3(-4.f, 1.f, 0.f), 1.f, std::make_shared<Lambertian>(vec3(0.4f, 0.2f, 0.1f)));
	hittables[idx++] = std::make_shared<Sphere>(
		vec3(4.f, 1.f, 0.f), 1.f, std::make_shared<Metal>(vec3(0.7f, 0.6f, 0.5f), 0.f));

	std::cout << "Done!\n";

	return std::make_shared<HittableList>(hittables, idx);
}

int main()
{
	// Timer t("Elapsed");

	constexpr int width = 200;
	constexpr int height = 100;
	constexpr int ns = 100;
	constexpr int num_channels = 3;

	const vec3 lower_left_corner(-2.f, -1.f, -1.f);
	const vec3 horizontal(4.f, 0.f, 0.f);
	const vec3 vertical(0.f, 2.f, 0.f);
	const vec3 origin(0.f, 0.f, 0.f);

	auto world = test_scene();

	vec3 lookfrom(13.f, 2.f, 3.f);
	vec3 lookat(0.f, 0.f, 0.f);
	vec3 cam_up(0.f, 1.f, 0.f);
	constexpr float dist_to_focus = 10.f;
	constexpr float aperture = 0.1f;
	constexpr float fov = 20.f;
	constexpr float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	Camera cam(lookfrom, lookat, cam_up, fov, aspect_ratio, aperture, dist_to_focus);

	std::mt19937 mt_engine(std::random_device{}());
	std::uniform_real_distribution<float> fdist(0.f, 0.999f);

	std::cout << "Generating image... ";

	std::vector<unsigned char> image(width * height * num_channels);
	for(int row = height - 1; row >= 0; row--)
	{
		for(int column = 0; column < width; column++)
		{
			vec3 col(0.f, 0.f, 0.f);
			for(int s = 0; s < ns; s++)
			{
				float u = (float(column) + fdist(mt_engine)) / float(width);
				float v = (float(row) + fdist(mt_engine)) / float(height);

				Ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.f);
				col += colour(r, world, 0);
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int y = height - row - 1;
			image[(column + y * width) * num_channels + 0] =
				static_cast<unsigned char>(int(255.99f * col.r()));
			image[(column + y * width) * num_channels + 1] =
				static_cast<unsigned char>(int(255.99f * col.g()));
			image[(column + y * width) * num_channels + 2] =
				static_cast<unsigned char>(int(255.99f * col.b()));
		}
	}
	std::cout << "Done!\n";

	std::cout << "Writing to file... ";

	std::string filename = "out.png";
	stbi_write_png(filename.c_str(), width, height, num_channels, &image[0], width * num_channels);

	std::cout << "Done!\n";
}
