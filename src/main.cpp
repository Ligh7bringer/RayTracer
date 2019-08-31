#include <fstream>
#include <iostream>

#include "ray.h"

bool hit_sphere(const vec3& centre, float radius, const ray& r)
{
	vec3 oc = r.origin() - centre;
	float a = dot(r.direction(), r.direction());
	float b = 2.f * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

vec3 colour(const ray& r)
{
	if(hit_sphere(vec3(0, 0, -1), 0.5, r))
		return vec3(1, 0, 0);

	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * static_cast<float>((unit_direction.y() + 1.0f));
	return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	int nx = 1200;
	int ny = 600;

	std::ofstream out;
	out.open("out.ppm");

	out << "P3\n" << nx << " " << ny << "\n255\n";

	const vec3 lower_left_corner(-2.f, -1.f, -1.f);
	const vec3 horizontal(4.f, 0.f, 0.f);
	const vec3 vertical(0.f, 2.f, 0.f);
	const vec3 origin(0.f, 0.f, 0.f);

	for(int j = ny - 1; j >= 0; j--)
	{
		for(int i = 0; i < nx; i++)
		{
			auto u = float(i) / float(nx);
			auto v = float(j) / float(ny);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			vec3 col = colour(r);
			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
}
