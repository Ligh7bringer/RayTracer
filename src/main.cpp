#include <fstream>
#include <iostream>
#include <limits>
#include <random>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

vec3 colour(const ray& r, hittable* world)
{
	hit_record rec;
	if(world->hit(r, 0.f, std::numeric_limits<float>::max(), rec))
	{
		return 0.5f * vec3(rec.normal.x() + 1.f, rec.normal.y() + 1.f, rec.normal.z() + 1.f);
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
	int nx = 1200;
	int ny = 600;
	int ns = 100;

	std::ofstream out;
	out.open("out.ppm");

	out << "P3\n" << nx << " " << ny << "\n255\n";

	const vec3 lower_left_corner(-2.f, -1.f, -1.f);
	const vec3 horizontal(4.f, 0.f, 0.f);
	const vec3 vertical(0.f, 2.f, 0.f);
	const vec3 origin(0.f, 0.f, 0.f);

	hittable* list[2];
	list[0] = new sphere(vec3(0.f, 0.f, -1.f), 0.5f);
	list[1] = new sphere(vec3(0.f, -100.5f, -1.f), 100.f);
	hittable* world = new hittable_list(list, 2);

	camera cam;

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

				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.f);
				col += colour(r, world);
			}

			col /= float(ns);
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
}
