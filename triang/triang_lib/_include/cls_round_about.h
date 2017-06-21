#pragma once

#include "cls_triangulation.h"

class cls_round_about
{
	cls_triangulation* const _link;
	cls_triangulation::add_cont* const _add;
public:
	explicit cls_round_about(cls_triangulation* const p_link, cls_triangulation::add_cont* const p_add);
	int operator()(const cls_dot& p_dot, const cls_dot& p_pivot, ptrdiff_t p_curr, ptrdiff_t p_prev) const;
};
