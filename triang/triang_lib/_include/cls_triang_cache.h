#pragma once

#include "cls_triang_node.h"

#define MIN_CACHE_GROW_COEF 3

class cls_triangulation;

class cls_triang_cache
{
	cls_triangulation* const _link;
	size_t _size;
	size_t* _cache;
	unsigned _scale;
	unsigned _growth;
	
	void enlarge();
	void check_link() const;
public:
	cls_triang_cache(cls_triangulation* const p_link, const unsigned p_scale, const unsigned p_growth = MIN_CACHE_GROW_COEF);
	cls_triang_cache(const cls_triang_cache& that);
	cls_triang_cache& operator=(const cls_triang_cache& that);
	void fill(const size_t p_index) const;
	size_t& set(const cls_dot& p_dot) const;
	size_t index(const cls_dot& p_dot) const;
	cls_triang_node& operator[](const cls_dot& p_dot) const;
	cls_triang_node& operator()(const size_t& i, const size_t& j) const;
	size_t size() const;
	void growth(const size_t p_added_dots);
};
