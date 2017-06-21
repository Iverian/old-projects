#pragma once

#include "cls_dot.h"
#include "cls_vertex.h"
#include "cls_image.h"
#include "cls_drawable.h"

#include <array>

enum class enu_belong {
    OUTSIDE = 0,
    INSIDE = 1,
    EDGE_A = -1,
    EDGE_B = -2,
    EDGE_C = -3
};

class cls_triangle : public cls_drawable {
    cls_dot _vertex[3];
    bool _exist;

public:
	using edge = std::array<cls_dot, 2>;

    cls_triangle();
    bool is_exist() const;
    cls_triangle(const cls_dot& a, const cls_dot& b, const cls_dot& c);
    explicit cls_triangle(cls_dot const p_vertex[3]);
    const cls_dot& operator[](cls_vertex index) const; //принимает номер вершины в пределах 1..3
	const cls_dot& operator[](enu_vertex index) const;
	const cls_dot& at(cls_vertex index) const;
	const cls_dot& at(enu_vertex index) const;

	edge get_edge(cls_vertex index) const;
	cls_vertex is_vertex(const cls_dot& x) const; //0 - не вершина, иначе номер вершины в 1..3
    enu_belong dot_belong(const cls_dot& dot) const;
	bool operator==(const cls_triangle& op2) const;
	bool operator!=(const cls_triangle& op2) const;
	bool delone_check_dot(const cls_dot& p_dot) const;
	cls_image& draw(cls_image& p_img, const cls_image::color_type& color) const override;

	class cls_triangle_iterator;
    typedef cls_triangle_iterator const_iterator;
    cls_triangle_iterator begin() const;
    cls_triangle_iterator end() const;
};

class cls_triangle::cls_triangle_iterator {
	const cls_triangle* _link;
	cls_vertex _ver;
	inline void check_link() const;

public:
	cls_triangle_iterator(const cls_triangle* p_link, const enu_vertex p_ver);
	cls_triangle_iterator(const cls_triangle_iterator& that);
	cls_triangle_iterator& operator=(const cls_triangle_iterator& that);
	cls_triangle_iterator& operator++();
	cls_triangle_iterator operator++(int);
	cls_triangle_iterator cycle(const unsigned p_times = 1);
	cls_triangle_iterator const_cycle(const unsigned p_times = 1) const;
	operator int() const;
	operator cls_vertex() const;
	const cls_dot& operator*() const;
	const cls_dot* operator->() const;
	bool operator==(const cls_triangle_iterator op2) const;
	bool operator!=(const cls_triangle_iterator op2) const;
};
