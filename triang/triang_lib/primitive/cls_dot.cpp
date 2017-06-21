#include "cls_dot.h"
#include "utility.h"

cls_dot::cls_dot(const int p_x, const int p_y)
    : _x(p_x)
    , _y(p_y)
{
}

cls_dot::cls_dot(const std::initializer_list<int>& p_list)
    : _x(0)
    , _y(0)
{
    if (p_list.size() >= 1)
        _x = *p_list.begin();
    if (p_list.size() >= 2)
        _y = *(p_list.begin() + 1);
}

cls_dot::cls_dot(const cls_dot& that)
    : cls_dot(that._x, that._y)
{
}

cls_dot& cls_dot::operator=(const cls_dot& that)
{
    _x = that._x;
    _y = that._y;
    return *this;
}

int cls_dot::x() const
{
    return _x;
}

int cls_dot::y() const
{
    return _y;
}

bool cls_dot::operator==(const cls_dot& op2) const
{
    return (_x == op2._x) && (_y == op2._y);
}

bool cls_dot::operator!=(const cls_dot& op2) const
{
    return !(*this == op2);
}

cls_image& cls_dot::draw(cls_image& p_img, const cls_image::color_type& p_color) const
{
	p_img.draw_line({ _x - draw_rad, _y }, { _x + draw_rad, _y }, p_color);
	p_img.draw_line({ _x, _y - draw_rad }, { _x, _y + draw_rad }, p_color);
	return p_img;
}

int sqr_distance(const cls_dot& op1, const cls_dot& op2)
{
    return sqr(op2._x - op1._x) + sqr(op2._y - op1._y);
}

bool on_same_line(const cls_dot& a, const cls_dot& b, const cls_dot& c)
{
	int x[] = { b.x() - a.x(), c.y() - a.y(), b.y() - a.y(), c.x() - a.x() };
	return x[0] * x[1] - x[2] * x[3] == 0;
}
