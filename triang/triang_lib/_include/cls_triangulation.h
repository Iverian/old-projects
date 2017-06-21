#pragma once

#define NOT_DEF ptrdiff_t(-1)

#include "cls_image.h"
#include "cls_triangle.h"
#include "cls_triang_cache.h"
#include "cls_triang_node.h"

#include <vector>

typedef std::vector<cls_dot> dots_cont;
typedef dots_cont::iterator dot_iter;

class cls_triangulation : cls_drawable {
	friend class cls_round_about;

	typedef std::vector<cls_triang_node>::iterator tri_iter;

    std::vector<cls_triang_node> _cont;
	cls_triang_cache _cache;
	size_t _dots_total;
public:

	typedef ptrdiff_t index_type;
	typedef std::vector<cls_triang_node>::size_type size_type;
	typedef std::vector<ptrdiff_t> add_cont;

    explicit cls_triangulation(dots_cont& p_dots, unsigned _scale);
	explicit cls_triangulation(cls_triangle& p_first, unsigned p_scale);

	cls_triang_node& operator[](index_type index);
	const cls_triang_node& operator[](index_type index) const;
	cls_triang_node& at(index_type p_index);
	const cls_triang_node& at(index_type p_index) const;
	index_type get_pos(const cls_triang_node& p_node) const;
	size_type size() const;

	void add_dot(const cls_dot& p_dot);
    cls_image& draw(cls_image& p_image, const cls_image::color_type& p_color) const override;
	tri_iter begin();
	tri_iter end();
private:
	index_type add_node(cls_triang_node&& p_node);
	index_type add_node(const cls_triang_node& p_node);
	std::tuple<index_type, std::vector<index_type>> add_dot_inside(const cls_dot& p_dot, const index_type p_found_tri);
	std::tuple<index_type, std::vector<index_type>> add_dot_edge(const cls_dot& p_dot, const index_type p_found_tri, const cls_vertex p_vertex);
	std::tuple<index_type, std::vector<index_type>> add_dot_outside(const cls_dot& p_dot, const index_type p_found_tri, const cls_vertex p_edge);
	void add_first_tri(dots_cont& p_dots);
	void delone_rebuild_node(const index_type p_index);
};


