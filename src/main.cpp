#include <fstream>
#include <iostream>
#include <limits>
#include <random>

#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

vec3 colour(const Ray& r, Hittable* world, int depth)
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

int main()
{
	int nx = 800;
	int ny = 400;
	int ns = 100;

	std::ofstream out;
	out.open("out.ppm");

	out << "P3\n" << nx << " " << ny << "\n255\n";

	const vec3 lower_left_corner(-2.f, -1.f, -1.f);
	const vec3 horizontal(4.f, 0.f, 0.f);
	const vec3 vertical(0.f, 2.f, 0.f);
	const vec3 origin(0.f, 0.f, 0.f);

	constexpr int list_size = 5;
	Hittable* list[list_size];
	float R = static_cast<float>((M_PI / 4.0));
	list[0] = new Sphere(vec3(0.f, 0.f, -1.f), 0.5f, new Lambertian(vec3(0.1f, 0.2f, 0.5f)));
	list[1] = new Sphere(vec3(0.f, -100.5f, -1.f), 100.f, new Lambertian(vec3(0.8f, 0.8f, 0.f)));
	list[2] = new Sphere(vec3(1.f, 0.f, -1.f), 0.5f, new Metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new Sphere(vec3(-1.f, 0.f, -1.f), 0.5f, new Dielectric(1.5f));
	list[4] = new Sphere(vec3(-1.f, 0.f, -1.f), -0.45f, new Dielectric(1.5f));
	Hittable* world = new HittableList(list, list_size);

	Camera cam(vec3(-2.f, 2.f, 1.f),
			   vec3(0.f, 0.f, -1.f),
			   vec3(0.f, 1.f, 0.f),
			   90.f,
			   float(nx) / (float)(ny));

	std::mt19937 mt_engine(std::random_device{}());
	std::uniform_real_distribution<float> fdist(0.f, 0.999f);

	for(int j = ny - 1; j >= 0; j--)
	{
		for(int i = 0; i < nx; i++)
		{
			vec3 col(0.f, 0.f, 0.f);
			for(int s = 0; s < ns; s++)
			{
				float u = (float(i) + fdist(mt_engine)) / float(nx);
				float v = (float(j) + fdist(mt_engine)) / float(ny);

				Ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.f);
				col += colour(r, world, 0);
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
}
