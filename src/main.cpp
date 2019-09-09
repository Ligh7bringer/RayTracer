#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "camera.h"
#include "scene_factory.h"
#include "timer.h"
#include "util.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main()
{
	Timer t("Elapsed");

	constexpr int width = 600;
	constexpr int height = 300;
	constexpr int num_samples = 100; // per pixel
	constexpr int num_channels = 3;

	const vec3 lower_left_corner(-2.f, -1.f, -1.f);
	const vec3 horizontal(4.f, 0.f, 0.f);
	const vec3 vertical(0.f, 2.f, 0.f);
	const vec3 origin(0.f, 0.f, 0.f);

	vec3 lookfrom(13.f, 2.f, 3.f);
	vec3 lookat(0.f, 0.f, 0.f);
	vec3 cam_up(0.f, 1.f, 0.f);
	constexpr float dist_to_focus = 10.f;
	constexpr float aperture = 0.0f;
	constexpr float fov = 20.f;
	constexpr float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	Camera cam(lookfrom, lookat, cam_up, fov, aspect_ratio, aperture, dist_to_focus, 0.f, 1.f);

	std::mt19937 mt_engine(std::random_device{}());
	std::uniform_real_distribution<float> fdist(0.f, 0.999f);

	std::cout << "Generating scene... ";
	auto world = SceneFactory::two_perlin_spheres();
	std::cout << "Done! \n";

	std::cout << "Generating image... ";

	std::vector<unsigned char> image(width * height * num_channels);
	for(int row = height - 1; row >= 0; row--)
	{
		for(int column = 0; column < width; column++)
		{
			vec3 col(0.f, 0.f, 0.f);
			for(int s = 0; s < num_samples; s++)
			{
				float u = (float(column) + fdist(mt_engine)) / float(width);
				float v = (float(row) + fdist(mt_engine)) / float(height);

				Ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.f);
				col += Util::colour(r, world, 0);
			}

			col /= float(num_samples);
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
