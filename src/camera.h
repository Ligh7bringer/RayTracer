#pragma once

#include <cmath>
#include <random>

#include "ray.h"

constexpr double M_PI = 3.14159265358979323846; // pi

class Camera
{
public:
	Camera(vec3 lookfrom,
		   vec3 lookat,
		   vec3 vup,
		   float vfov,
		   float aspect,
		   float aperture,
		   float focus_dist)
	{
		lens_radius = aperture / 2.f;
		float theta = vfov * static_cast<float>(M_PI / 180.0);
		float half_height = tan(theta / 2.f);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner =
			origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2.f * half_width * focus_dist * u;
		vertical = 2.f * half_height * focus_dist * v;
	}

	Ray get_ray(float s, float t)
	{
		vec3 rd = lens_radius * random_in_unit_disc();
		vec3 offset = u * rd.x() + v * rd.y();

		return Ray(origin + offset,
				   lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}

private:
	vec3 random_in_unit_disc()
	{
		vec3 p;
		std::mt19937 mt_engine(std::random_device{}());
		std::uniform_real_distribution<float> fdist(0.f, 0.999f);

		do
		{
			p = 2.f * vec3(fdist(mt_engine), fdist(mt_engine), 0.f) - vec3(1.f, 1.f, 0.f);
		} while(dot(p, p) >= 1.f);

		return p;
	}

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lens_radius;
};
