#include "utility.h"
#include "cls_image.h"

#include <ctime>
#include <fstream>

Mod_counter::Mod_counter(int p_base)
    : _base(p_base)
{
}

int Mod_counter::next(int x) const
{
	return (++x) % _base;
}

int Mod_counter::prev(int x) const
{
	if (x == 0)
		x = _base - 1;
	else
		x = (--x) % _base;
	return x;
}

bool same_side(const cls_dot& dot, const cls_triangle& tri, cls_vertex ver)
{
    auto cur = tri[ver];
    auto edge = get_edge(ver);
    auto A = edge[0].y() - edge[1].y(),
    	 B = edge[1].x() - edge[0].x();
    long C = A*edge[0].x() + B*edge[0].y();
    return signbit(A*cur.x() + B*cur.y() - C) == signbit(A*dot.x() + B*dot.y() - C);
}

bool file_exist(const std::string& p_filename)
{
    std::ifstream f(p_filename);
    return f.is_open();
}

double angle(const std::array<cls_dot, 3>& x)
{
    auto a = uint64_t(x[0].x() - x[1].x()) * (x[2].x() - x[1].x()) + (x[0].y() - x[1].y()) * (x[2].y() - x[1].y());
    auto b = sqrt(sqr_distance(x[0], x[1]) * sqr_distance(x[1], x[2]));
    return (180 * a) / (pi * b);
}

bool is_hexagon_convex(const std::array<cls_dot, 6>& p_hex)
{
	auto retval = true;
	Mod_counter c(6);
	for (auto i = 0; i < 6; ++i) {
		auto a = angle({ p_hex[c.prev(i)], p_hex[i], p_hex[c.next(i)] });
		if (a > 180 || dbl_eq(a, 180))
		{
			retval = false;
			break;
		}
	}
	return retval;
}

cls_rand_letter::cls_rand_letter()
    : _eng(static_cast<unsigned>(time(nullptr)))
{
}

char cls_rand_letter::next()
{
    return (_eng() % 2) ? ('0' + _eng() % 10) : ((_eng() % 2 ? 'A' : 'a') + _eng() % 26);
}
