#include "cls_triang_cache.h"
#include "exceptions.h"
#include "utility.h"
#include "cls_triangulation.h"

#define _(i, j) [(i)*_size + (j)]
#define __(i, j) [(i)*(_size * 2) + (j)]

void cls_triang_cache::enlarge()
{
    size_t* tmp;
    auto new_array = new size_t[sqr(_size * 2)];
	memset(new_array, 0, sqr(_size * 2));
    for (size_t i = 0; i < _size; ++i)
        for (size_t j = 0; j < _size; ++j)
            new_array __(2 * i, 2 * j) = new_array __(2 * i + 1, 2 * j) = new_array __(2 * i, 2 * j + 1) = new_array __(2 * i + 1, 2 * j + 1) = _cache _(i, j);
    _size *= 2;
    tmp = _cache;
    _cache = new_array;
    delete[] tmp;
}

void cls_triang_cache::check_link() const
{
	if (_link == nullptr)
		THROW(cls_invalid_link);
}

cls_triang_cache::cls_triang_cache(cls_triangulation* const p_link, const unsigned p_scale, const unsigned p_growth)
    : _link(p_link)
    , _size(2)
    , _scale(p_scale)
    , _growth(p_growth)
{
    _cache = new size_t[4];
    for (auto i = 0; i < 2; ++i)
        for (auto j = 0; j < 2; ++j)
            _cache _(i, j) = 0;
}

cls_triang_cache::cls_triang_cache(const cls_triang_cache& that)
    : _link(that._link)
    , _size(that._size)
    , _cache(new size_t[sqr(_size)])
    , _scale(that._scale)
    , _growth(that._growth)
{
    for (size_t i = 0; i < _size; ++i)
        for (size_t j = 0; j < _size; ++j)
            _cache _(i, j) = that._cache _(i, j);
}

cls_triang_cache& cls_triang_cache::operator=(const cls_triang_cache& that)
{
	check_link();
	if (_link != that._link)
		THROW(cls_invalid_link);
    _size = that._size;
    _cache = new size_t[sqr(_size)];
    _scale = that._scale;
    _growth = that._growth;
    for (size_t i = 0; i < _size; ++i)
        for (size_t j = 0; j < _size; ++j)
            _cache _(i, j) = that._cache _(i, j);
    return *this;
}

void cls_triang_cache::fill(const size_t p_index) const
{
	for (size_t i = 0; i < _size; ++i)
		for (size_t j = 0; j < _size; ++j)
			_cache _(i, j) = p_index;
}

size_t& cls_triang_cache::set(const cls_dot& p_dot) const
{
	auto i = size_t((p_dot.x() * _size) / _scale);
	auto j = size_t((p_dot.y() * _size) / _scale);
	return _cache _(i, j);
}

size_t cls_triang_cache::index(const cls_dot& p_dot) const
{
	auto i = size_t((p_dot.x() * _size) / _scale);
	auto j = size_t((p_dot.y() * _size) / _scale);
	return _cache _(i, j);
}

cls_triang_node& cls_triang_cache::operator[](const cls_dot& p_dot) const
{
	auto i = size_t((p_dot.x() * _size) / _scale);
	auto j = size_t((p_dot.y() * _size) / _scale);
    return operator()(i, j);
}

cls_triang_node& cls_triang_cache::operator()(const size_t& i, const size_t& j) const
{
	check_link();
    if (i < _size && j < _size)
        return (*_link)[_cache _(i, j)];
    THROW(cls_out_of_range);
}

size_t cls_triang_cache::size() const
{
	check_link();
    return _size;
}

void cls_triang_cache::growth(const size_t p_added_dots)
{
	check_link();
    if (p_added_dots == sqr(_size) * _growth)
        enlarge();
}
