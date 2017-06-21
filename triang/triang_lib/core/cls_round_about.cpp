#include "cls_round_about.h"
#include "utility.h"
#include "cls_dbg_draw.h"

cls_round_about::cls_round_about(cls_triangulation* const p_link, cls_triangulation::add_cont* const p_add)
	: _link(p_link)
	, _add(p_add)
{
}

#define AT(x) (_link->at(x))

int cls_round_about::operator()(const cls_dot& p_dot, const cls_dot& p_pivot, ptrdiff_t p_curr, ptrdiff_t p_prev) const
{
	auto retval = 0;
	for (auto i = AT(p_curr).begin(); i != AT(p_curr).end(); ++i) {
		if (*i == p_pivot || AT(p_curr).get_neigh(i) == AT(p_prev))
			continue;
		if (AT(p_curr).get_neigh(i).is_exist()) {
			retval += operator()(p_dot, p_pivot, AT(p_curr).get_neigh_index(i), p_curr);
			break;
		}
		if (!same_side(p_dot, AT(p_curr), i)) {
			auto edge = AT(p_curr).get_edge(i);
			auto added = _link->add_node(cls_triang_node(_link, p_dot, edge[0], edge[1]));
			AT(added).set_neigh(AT(p_curr));
			_add->push_back(added);

			DBG__DRAW("outside_roundabout_tripair",
				*_link,
				color::blue, AT(p_curr),
				color::green, AT(added)
			);

			++retval;
			retval += operator()(p_dot, AT(p_curr)[AT(p_curr).is_neigh(AT(p_prev))], p_curr, added);
			break;
		}
	}
	return retval;
}
