#include "cls_image.h"
#include "cls_triangle.h"

static constexpr auto rgb = 3;

using namespace cimg_library;

inline cls_image::cls_image()
    : image()
{
}

cls_image::cls_image(const size_t x, const size_t y, const unsigned margin, const pixel& value)
    : image(x + 2 * margin, y + 2 * margin, 1, rgb, value)
    , _margin(margin)
{
}

cls_image::cls_image(const char* const filename)
    : image(filename)
{
}

cls_image& cls_image::draw_dot(const cls_dot& dot, const cls_image::color_type& color)
{
    image::draw_point(dot.x() + _margin, dot.y() + _margin, convert(color).data());
    return *this;
}

cls_image& cls_image::draw_line(const cls_dot& dot1, const cls_dot& dot2, const cls_image::color_type& color)
{
    image::draw_line(dot1.x() + _margin, dot1.y() + _margin, dot2.x() + _margin, dot2.y() + _margin, convert(color).data());
    return *this;
}

cls_image& cls_image::draw_triangle(const cls_triangle& tri, const cls_image::color_type& color_tri)
{
	image::draw_triangle(
		tri[enu_vertex::A].x() + _margin, tri[enu_vertex::A].y() + _margin,
		tri[enu_vertex::B].x() + _margin, tri[enu_vertex::B].y() + _margin,
		tri[enu_vertex::C].x() + _margin, tri[enu_vertex::C].y() + _margin,
		convert(color_tri).data(), 1.f, ~0U);
	return *this;
}

cls_image& cls_image::draw_triangle(const cls_triangle& tri, const cls_image::color_type& color_tri, const cls_image::color_type& color_ver)
{
	draw_triangle(tri, color_tri);
    for (auto& i : tri)
        draw_circle(i, 3, color_ver);
    return *this;
}

cls_image& cls_image::draw_circle(const cls_dot& dot, int rad, const cls_image::color_type& color)
{
    image::draw_circle(dot.x() + _margin, dot.y() + _margin, rad, convert(color).data());
    return *this;
}

cls_image& cls_image::draw_outlined_circle(const cls_dot& dot, int rad, const cls_image::color_type& color)
{
    draw_circle(dot, 3, color);
    image::draw_circle(dot.x() + _margin, dot.y() + _margin, rad, convert(color).data(), 1.f, ~0U);
    return *this;
}

cls_image& cls_image::save(const char* filename, const int number, const int digits)
{
    image::save(filename, number, digits);
    return *this;
}

cls_image& cls_image::save(const std::string& filename, const int number, const int digits)
{
    save(filename.c_str(), number, digits);
    return *this;
}

std::array<pixel, 3> cls_image::convert(const color_type& p_color)
{
	return std::array<pixel, 3>{(p_color >> 16) & 0xff, (p_color >> 8) & 0xff, p_color & 0xff};
}
