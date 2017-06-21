#pragma once

#include "cls_triangle.h"

#include <random>
#include <cmath>

#define dbl_eq(x, y) (std::fabs((x) - (y)) < 1e-9)

static constexpr auto pi = 3.141592653589793238462643383;

template <class T>
auto sqr(T x) -> decltype(x * x) { return x * x; }

template <class T>
bool signbit(T x)
{
    return (x < T()) ? true : false;
}

struct Mod_counter
{
	explicit Mod_counter(int p_base);
	int next(int x) const;
	int prev(int x) const;
private:
	int _base;
};

bool same_side(const cls_dot& dot, const cls_triangle& tri, cls_vertex ver);
bool file_exist(const std::string& p_filename);
double angle(const std::array<cls_dot, 3>& p_dots);
bool is_hexagon_convex(const std::array<cls_dot, 6>& p_hex);

struct cls_rand_letter
{
	cls_rand_letter();
	char next();
private:
	std::minstd_rand0 _eng;
};