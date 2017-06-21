#include "cls_triangle.h"
#include "exceptions.h"
#include "utility.h"

template <class T>
bool signbit_fraction(T x, T y)
{
    return signbit(x) != signbit(y);
}

cls_triangle::cls_triangle()
    : _vertex{ cls_dot(), cls_dot(), cls_dot() }
    , _exist(false)
{
}

bool cls_triangle::is_exist() const
{
    return _exist;
}

cls_triangle::cls_triangle(const cls_dot& a, const cls_dot& b, const cls_dot& c)
    : cls_triangle()
{
    if (!on_same_line(a, b, c)) {
        _exist = true;
        double x1 = a.x() - b.x();
        double y1 = a.y() - b.y();
        double x2 = c.x() - b.x();
        double y2 = c.y() - b.y();
        _vertex[0] = a;
        if (x1 * y2 - y1 * x2 >= 0) {
            _vertex[1] = b;
            _vertex[2] = c;
        } else {
            _vertex[1] = c;
            _vertex[2] = b;
        }
    }
}

cls_triangle::cls_triangle(cls_dot const p_vertex[3])
    : cls_triangle(p_vertex[0], p_vertex[1], p_vertex[2])
{
}

const cls_dot& cls_triangle::operator[](cls_vertex index) const
{
    if (_exist) {
        if (index != cls_vertex(enu_vertex::NIL))
            return _vertex[int(index)];
        THROW(cls_out_of_range);
    }
    THROW(cls_bad_triangle);
}

const cls_dot& cls_triangle::operator[](enu_vertex index) const
{
    return (*this)[cls_vertex(index)];
}

const cls_dot& cls_triangle::at(cls_vertex index) const
{
    return (*this)[index];
}

const cls_dot& cls_triangle::at(enu_vertex index) const
{
    return (*this)[cls_vertex(index)];
}

cls_triangle::edge cls_triangle::get_edge(cls_vertex index) const
{
    return { (*this)[index.const_cycle()], (*this)[index.const_cycle(2)] };
}

cls_vertex cls_triangle::is_vertex(const cls_dot& x) const
{
    cls_vertex retval(enu_vertex::NIL);
    if (_exist) {
        for (auto i = begin(); i != end(); ++i)
            if (x == _vertex[int(i)]) {
                retval = i;
                break;
            }
    } else
        THROW(cls_bad_triangle);
    return retval;
}

inline enu_belong vertex_to_bel(const cls_vertex& x)
{
    auto retval = enu_belong::OUTSIDE;
    switch (enu_vertex(x)) {
    case enu_vertex::A:
        retval = enu_belong::EDGE_A;
        break;
    case enu_vertex::B:
        retval = enu_belong::EDGE_B;
        break;
    case enu_vertex::C:
        retval = enu_belong::EDGE_C;
        break;
    default:
        break;
    }
    return retval;
}

enu_belong cls_triangle::dot_belong(const cls_dot& dot) const
{

    auto retval = enu_belong::OUTSIDE;
    if (_exist) {
        for (auto i = begin(); i != end(); ++i)
            if (on_same_line(dot, *i.const_cycle(), *i.const_cycle(2)) &&
            	sqr_distance(dot, *i.const_cycle()) <= sqr_distance(*i.const_cycle(), *i.const_cycle(2))) {
                retval = vertex_to_bel(i);
                break;
            }
		if (retval == enu_belong::OUTSIDE) {
			auto stat = true;
			for (auto i = begin(); i != end(); ++i)
				if (!same_side(dot, *this, i)) {
					stat = false;
					break;
				}
			if (stat)
				retval = enu_belong::INSIDE;
		}
        return retval;
    }
    THROW(cls_bad_triangle);
}

bool cls_triangle::operator==(const cls_triangle& op2) const
{
    return _vertex[0] == op2._vertex[0] && _vertex[1] == op2._vertex[1] && _vertex[2] == op2._vertex[2];
}

bool cls_triangle::operator!=(const cls_triangle& op2) const
{
    return !(*this == op2);
}

#define x0 (p_dot.x())
#define x1 (edge[1].x())
#define x2 ((*i).x())
#define x3 (edge[0].x())
#define y0 (p_dot.y())
#define y1 (edge[1].y())
#define y2 ((*i).y())
#define y3 (edge[0].y())

bool cls_triangle::delone_check_dot(const cls_dot& p_dot) const
{
    auto retval = false;
    for (auto i = begin(); i != end(); ++i)
        if (!same_side(p_dot, *this, i)) {
            cls_dot edge[] = { *i.const_cycle(), *i.const_cycle(2) };

            intmax_t s[4] = {
                0,
                (x2 - x1) * (x2 - x3) + (y2 - y1) * (y2 - y3),
                (x0 - x1) * (x0 - x3) + (y0 - y1) * (y0 - y3),
                0
            };
            if (s[1] < 0 && s[2] < 0)
                retval = false;
            else if (s[1] >= 0 && s[2] >= 0)
                retval = true;
            else {
                s[0] = abs((x0 - x1) * (y0 - y3) - (x0 - x3) * (y0 - y1));
                s[3] = abs((x2 - x3) * (y2 - y1) - (x2 - x1) * (y2 - y3));
                retval = s[0] * s[1] + s[2] * s[3] >= 0;
            }
            break;
        }
    return retval;
}

#undef x0
#undef x1
#undef x2
#undef x3
#undef x0
#undef y1
#undef y2
#undef y3

cls_image& cls_triangle::draw(cls_image& p_img, const cls_image::color_type& p_color) const
{
    return p_img.draw_triangle(*this, p_color);
}

cls_triangle::cls_triangle_iterator cls_triangle::begin() const
{
    return cls_triangle_iterator(this, enu_vertex::A);
}

cls_triangle::cls_triangle_iterator cls_triangle::end() const
{
    return cls_triangle_iterator(this, enu_vertex::NIL);
}
