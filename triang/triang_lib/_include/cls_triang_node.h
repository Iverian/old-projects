#pragma once

#include "cls_triangle.h"

class cls_triangulation;

class cls_triang_node : public cls_triangle {
	cls_triangulation* const _link;
	ptrdiff_t _neigh[3];

	inline void check_link() const;
public:
	friend class cls_triangulation;

	cls_triang_node();
	explicit cls_triang_node(cls_triangulation* const p_link);
	cls_triang_node(cls_triangulation* const p_link, const cls_triangle& p_tri);
	cls_triang_node(cls_triangulation* const p_link, const cls_dot& p_a, const cls_dot& p_b, const cls_dot& p_c);
	cls_triang_node(cls_triangulation* const p_link, const cls_dot p_arr[3]);
	cls_triang_node(const cls_triang_node& that);
	cls_triang_node& operator=(const cls_triang_node& that);
	bool operator==(const cls_triang_node& op2) const;
	bool operator!=(const cls_triang_node& op2) const;
	bool delone_check(const cls_triangle& p_tri) const;
	cls_triang_node& get_neigh(const cls_vertex p_ver) const;
	cls_triang_node& get_neigh(const enu_vertex p_ver) const;
	cls_triang_node& get_neigh(const cls_dot& p_dot) const;
	ptrdiff_t get_neigh_index(const cls_vertex p_ver) const;
	ptrdiff_t get_neigh_index(const enu_vertex p_ver) const;
	cls_vertex is_neigh(const cls_triangle& p_tri) const;
	void set_neigh(cls_triang_node& p_node);
	void set_neigh(cls_triang_node& op1, cls_triang_node& op2);
	void set_neigh(cls_triang_node& op1, cls_triang_node& op2, cls_triang_node& op3);
	int count_neigh() const;
	cls_dot diff_ver(const cls_vertex p_ver) const;
	cls_dot diff_ver(const enu_vertex p_ver) const;
	cls_dot diff_ver(const cls_triangle& p_tri) const;
};

