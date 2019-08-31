#pragma once

#include "ray.h"

class camera
{
public:
	camera()
		: origin(vec3(0.f, 0.f, 0.f))
		, lower_left_corner(vec3(-2.f, -1.f, -1.f))
		, horizontal(4.f, 0.f, 0.f)
		, vertical(0.f, 2.f, 0.f)
	{}

	ray get_ray(float u, float v)
	{
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};
