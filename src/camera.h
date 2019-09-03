#pragma once

#include <cmath>

#include "ray.h"

constexpr double M_PI = 3.14159265358979323846; // pi

class Camera
{
public:
	Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect)
	{
		vec3 u, v, w;
		float theta = vfov * static_cast<float>(M_PI / 180.0);
		float half_height = tan(theta / 2.f);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		//lower_left_corner = vec3(-half_width, -half_height, -1.f);
		lower_left_corner = origin - half_width * u - half_height * v - w;
		horizontal = 2.f * half_width * u;
		vertical = 2.f * half_height * v;
	}

	Ray get_ray(float u, float v)
	{
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};
