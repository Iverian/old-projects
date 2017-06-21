#pragma once

#include "CImg.h"

#include <array>

class cls_dot;
class cls_triangle;

typedef unsigned char pixel;
typedef cimg_library::CImg<pixel> image;

class cls_image : public image {
    unsigned _margin = 0;

public:
	typedef uint32_t color_type;

    cls_image();
    cls_image(const size_t x, const size_t y, const unsigned margin = 0, const pixel& value = -1);
    explicit cls_image(const char* const filename);
    cls_image& draw_dot(const cls_dot& dot, const color_type& color);
    cls_image& draw_line(const cls_dot& dot1, const cls_dot& dot2, const color_type& color);
	cls_image& draw_triangle(const cls_triangle& tri, const color_type& color_tri);
    cls_image& draw_triangle(const cls_triangle& tri, const color_type& color_tri, const color_type& color_ver);
    cls_image& draw_circle(const cls_dot& dot, int rad, const color_type& color);
    cls_image& draw_outlined_circle(const cls_dot& dot, int rad, const color_type& color);
    cls_image& save(const char* filename, const int number = -1, const int digits = 6);
    cls_image& save(const std::string& filename, const int number = -1, const int digits = 6);
private:
	static std::array<pixel, 3> convert(const color_type& p_color);
};

namespace color {
	constexpr cls_image::color_type red = 0xff0000;
	constexpr cls_image::color_type green = 0x00ff00;
	constexpr cls_image::color_type blue = 0x0000ff;
	constexpr cls_image::color_type black = 0x0;
};
