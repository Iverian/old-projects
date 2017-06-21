#include "dots_file.h"

#include <cls_triangulation.h>
#include <iostream>

#define SCALE 1000
#define DOTS_FILE "dots"

int main()
{
	dots_cont dots;
	auto n = 0;
	std::cin >> n;
	unsigned scale = SCALE;
	if (n == 0)
		scale = dots_get(dots, DOTS_FILE);
	else {
		dots = dots_gen(n, SCALE);
		dots_save(dots, scale, DOTS_FILE);
	}
	cls_image img_dots(scale, scale, 50);
	for (auto& i : dots)
		i.draw(img_dots, color::black);
	img_dots.save("dots.bmp");

	cls_triangulation triang(dots, scale);

	cls_image img(scale, scale, 50);
	triang.draw(img, color::black);
	img.save("out.bmp");

	return 0;
}